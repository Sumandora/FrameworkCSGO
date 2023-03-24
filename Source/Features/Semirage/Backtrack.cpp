#include "../Semirage.hpp"

#include <deque>
#include <map>
#include <ranges>
#include <vector>

#include "../General.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/NetworkFlows.hpp"

#include "../../Utils/Trigonometry.hpp"

bool Features::Semirage::Backtrack::enabled = false;
float Features::Semirage::Backtrack::time = 1.0f;
bool Features::Semirage::Backtrack::accountForOutgoingPing = false;
bool Features::Semirage::Backtrack::friendlyFire = false;

#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / Memory::globalVars->interval_per_tick))
#define TICKS_TO_TIME(t) (Memory::globalVars->interval_per_tick * (t))

struct Tick {
	float simulationTime {};
	int tickCount {};
	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
};

float CalculateLerpTime()
{
	// https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/server/gameinterface.cpp#L3281
	float flUpdateRateValue = ConVarStorage::cl_updaterate->GetFloat();
	flUpdateRateValue = std::clamp(flUpdateRateValue, ConVarStorage::sv_minupdaterate->GetFloat(), ConVarStorage::sv_maxupdaterate->GetFloat());

	if (ConVarStorage::cl_interpolate->GetBool()) {
		float flLerpRatio = ConVarStorage::cl_interp_ratio->GetFloat();
		if (flLerpRatio == 0)
			flLerpRatio = 1.0f;
		const float flLerpAmount = ConVarStorage::cl_interp->GetFloat();

		const float min = ConVarStorage::sv_client_min_interp_ratio->GetFloat();
		if (min != -1) { // They forgot to check for max != -1, didn't they?
			flLerpRatio = std::clamp(flLerpRatio, min, ConVarStorage::sv_client_max_interp_ratio->GetFloat());
		}
		return std::max(flLerpAmount, flLerpRatio / flUpdateRateValue);
	} else {
		return 0.0f;
	}
}

bool IsTickValid(Tick tick)
{
	// https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/server/player_lagcompensation.cpp#L246
	float correct = 0.0f;

	CNetChan* chan = Interfaces::engine->GetNetChannel();
	if (chan) {
		correct += chan->GetLatency(FLOW_INCOMING); // The server asks for OUTGOING, we have to turn this around, since we are the client.
		if (Features::Semirage::Backtrack::accountForOutgoingPing)
			correct += chan->GetLatency(FLOW_OUTGOING); // The server should account for this.
	}

	const float m_fLerpTime = CalculateLerpTime();

	correct += m_fLerpTime;

	correct = std::clamp(correct, 0.0f, ConVarStorage::sv_maxunlag->GetFloat() * Features::Semirage::Backtrack::time);

	const float flTargetTime = TICKS_TO_TIME(tick.tickCount) - m_fLerpTime;

	const float deltaTime = correct - (Memory::globalVars->curtime - flTargetTime);

	return fabs(deltaTime) <= 0.2f; // If our delta is higher than this the game will ignore our target time. We won't be able to hit anything
}

float CalculateFOVDistance(CBasePlayer* localPlayer, Vector viewangles, Vector b)
{
	Vector requiredView = Utils::CalculateView(localPlayer->GetEyePosition(), b);
	requiredView -= *localPlayer->AimPunchAngle() * ConVarStorage::weapon_recoil_scale->GetFloat();
	requiredView -= viewangles;
	requiredView.Wrap();

	return requiredView.Length();
}

std::map<int, std::vector<Tick>> ticks;

void Features::Semirage::Backtrack::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));

	if (!weapon)
		return;

	if (*weapon->NextPrimaryAttack() > Memory::globalVars->curtime || *localPlayer->WaitForNoAttack()) // TODO Rebuild those https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/shared/cstrike15/weapon_csbase.cpp#L990
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	float bestDistance = FLT_MAX;
	Tick bestTick {};

	std::erase_if(ticks, [&](const auto& pair) {
		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(pair.first));
		if (!player || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity() || !IsParticipatingTeam(*player->Team())) {
			return true;
		}

		if (!(friendlyFire || player->IsEnemy()))
			return true;

		std::vector<Tick> records = pair.second;

		Matrix3x4 boneMatrix[MAXSTUDIOBONES];
		if (!player->SetupBones(boneMatrix))
			return true;

		float currentDistance = CalculateFOVDistance(localPlayer, cmd->viewangles, boneMatrix[8].Origin());
#ifdef __clang__
		for (unsigned int index = records.size(); index > 0; index--) {
			auto& tick = records[index];
#else
		for (auto& tick : std::ranges::views::reverse(records)) {
#endif
			float delta = CalculateFOVDistance(localPlayer, cmd->viewangles, tick.boneMatrix[8].Origin());

			if (currentDistance > delta && bestDistance > delta) {
				bestDistance = delta;
				bestTick = tick;
			}
		}
		return false;
	});

	if (bestDistance < 5.0f && cmd->tick_count != bestTick.tickCount) {
		Features::General::EventLog::CreateReport("Trying to backtrack %d ticks", cmd->tick_count - bestTick.tickCount);
		cmd->tick_count = bestTick.tickCount;
	}
}

void Features::Semirage::Backtrack::FrameStageNotify()
{
	if (!enabled || !Interfaces::engine->IsInGame()) {
		ticks.clear();
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		if (!ticks.contains(i))
			ticks[i] = {};

		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == localPlayer || player->GetDormant() || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity()) {
			ticks[i].clear();
			continue;
		}

		if (!IsParticipatingTeam(*player->Team()) || !player->IsEnemy()) {
			ticks[i].clear();
			continue;
		}

		const float currentSimulationTime = *player->SimulationTime();
		if (!ticks[i].empty()) {
			if (ticks[i].back().simulationTime == currentSimulationTime)
				continue; // We don't have a new position yet
			ticks[i].erase(std::remove_if(ticks[i].begin(), ticks[i].end(), [&](const auto& tick) {
				return !IsTickValid(tick);
			}),
				ticks[i].end());
		}

		Tick tick;
		tick.simulationTime = currentSimulationTime;
		tick.tickCount = Memory::globalVars->tickcount;
		player->SetupBones(tick.boneMatrix);

		ticks[i].push_back(tick);
	}
}

void Features::Semirage::Backtrack::SetupGUI()
{
	if (!ConVarStorage::cl_lagcompensation->GetBool() || !ConVarStorage::sv_unlag->GetBool())
		ImGui::Text(xorstr_("Warning: Judging by convars, lag compensation is disabled."));
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Time"), &time, 0.0f, 1.0f, "%.2f");
	ImGui::Checkbox(xorstr_("Account for outgoing ping"), &accountForOutgoingPing);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);

	ImGui::Separator();

	ImGui::Text(xorstr_("You are backtracking up to a maximum of %.2f seconds"), ConVarStorage::sv_maxunlag->GetFloat() * Features::Semirage::Backtrack::time);
}

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Backtrack::Serializer, xorstr_("Backtrack"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Time"), time)
SERIALIZED_TYPE(xorstr_("Account for outgoing ping"), accountForOutgoingPing)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)
END_SERIALIZED_STRUCT
