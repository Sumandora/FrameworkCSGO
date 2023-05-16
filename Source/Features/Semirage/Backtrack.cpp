#include "Semirage.hpp"

#include <deque>
#include <map>
#include <ranges>
#include <vector>

#include "Features/General/General.hpp"

#include "GameCache.hpp"
#include "Interfaces.hpp"

#include "SDK/Definitions/InputFlags.hpp"
#include "SDK/Definitions/NetworkFlows.hpp"

#include "GUI/ImGuiColors.hpp"
#include "Utils/Trigonometry.hpp"

#include "Hooks/Game/GameFunctions.hpp"

#include "Features/Visuals/Visuals.hpp"

static bool enabled = false;
static float scale = 1.0f;
static bool accountForOutgoingPing = false;
static bool friendlyFire = false;
static bool visualize = false;

struct Tick {
	float simulationTime{};
	int tickCount{};
	Vector origin;
	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
};

float CalculateLerpTime()
{
	// https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/server/gameinterface.cpp#L3281
	float flUpdateRateValue = ConVarStorage::cl_updaterate()->GetFloat();
	flUpdateRateValue = std::clamp(flUpdateRateValue, ConVarStorage::sv_minupdaterate()->GetFloat(), ConVarStorage::sv_maxupdaterate()->GetFloat());

	if (ConVarStorage::cl_interpolate()->GetBool()) {
		float flLerpRatio = ConVarStorage::cl_interp_ratio()->GetFloat();
		if (flLerpRatio == 0)
			flLerpRatio = 1.0f;
		const float flLerpAmount = ConVarStorage::cl_interp()->GetFloat();

		const float min = ConVarStorage::sv_client_min_interp_ratio()->GetFloat();
		if (min != -1) { // They forgot to check for max != -1, didn't they?
			flLerpRatio = std::clamp(flLerpRatio, min, ConVarStorage::sv_client_max_interp_ratio()->GetFloat());
		}
		return std::max(flLerpAmount, flLerpRatio / flUpdateRateValue);
	}
	return 0.0f;
}

bool IsTickValid(const Tick& tick)
{
	// https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/server/player_lagcompensation.cpp#L246
	float correct = 0.0f;

	CNetChan* chan = Interfaces::engine->GetNetChannel();
	if (chan) {
		correct += chan->GetLatency(FLOW_INCOMING); // The server asks for OUTGOING, we have to turn this around, since we are the client.
		if (accountForOutgoingPing)
			correct += chan->GetLatency(FLOW_OUTGOING); // The server should account for this.
	}

	const float m_fLerpTime = CalculateLerpTime();

	correct += m_fLerpTime;

	correct = std::clamp(correct, 0.0f, ConVarStorage::sv_maxunlag()->GetFloat() * scale);

	const float flTargetTime = tick.tickCount * Memory::globalVars->interval_per_tick - m_fLerpTime;

	const float deltaTime = correct - (Memory::globalVars->curtime - flTargetTime);

	return fabs(deltaTime) <= 0.2f; // If our delta is higher than this the game will ignore our target time. We won't be able to hit anything
}

float CalculateFOVDistance(CBasePlayer* localPlayer, const Vector& viewangles, const Vector& b)
{
	Vector requiredView = Utils::CalculateView(localPlayer->GetEyePosition(), b);
	requiredView -= *localPlayer->AimPunchAngle() * 2.0f;
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
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));

	if (!weapon)
		return;
	if (*weapon->NextPrimaryAttack() > Memory::globalVars->curtime || *localPlayer->WaitForNoAttack()) // TODO Rebuild those https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/shared/cstrike15/weapon_csbase.cpp#L990
		return;

	const WeaponID defIndex = *weapon->WeaponDefinitionIndex();
	bool hasLimitedDistance = IsKnife(defIndex);

	if (!(cmd->buttons & IN_ATTACK))
		return;

	float bestDistance = FLT_MAX;
	int tickCount = 0;

	std::erase_if(ticks, [&](const auto& pair) {
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(pair.first));
		if (!player || !player->IsAlive() || *player->GunGameImmunity() || !IsParticipatingTeam(*player->Team())) {
			return true;
		}

		if (!(friendlyFire || player->IsEnemy(localPlayer)))
			return true;

		std::vector<Tick> records = pair.second;

		Matrix3x4* boneMatrix = player->SetupBones();
		// TODO TraceRay for tick selection

		float currentDistance;
		if (!hasLimitedDistance)
			currentDistance = CalculateFOVDistance(localPlayer, cmd->viewangles, boneMatrix[8].Origin());
		else
			currentDistance = (*localPlayer->Origin() - *player->Origin()).Length();

#ifdef __clang__
		for (size_t index = records.size(); index--;) {
			auto& tick = records[index];
#else
		for (auto& tick : std::ranges::views::reverse(records)) {
#endif
			float delta;
			if (!hasLimitedDistance)
				delta = CalculateFOVDistance(localPlayer, cmd->viewangles, tick.boneMatrix[8].Origin());
			else // If we are holding a hasLimitedDistance search for the closest tick
				delta = (*localPlayer->Origin() - tick.origin).Length();

			if (currentDistance > delta && bestDistance > delta) {
				bestDistance = delta;
				tickCount = tick.tickCount;
			}
		}
		return false;
	});

	if (tickCount > 0 && cmd->tick_count != tickCount && (hasLimitedDistance || bestDistance < 5.0f)) {
		Features::General::EventLog::CreateReport(xorstr_("Trying to backtrack %d ticks"), cmd->tick_count - tickCount);
		cmd->tick_count = tickCount;
	}
}

void Features::Semirage::Backtrack::FrameStageNotify()
{
	if (!enabled || !Interfaces::engine->IsInGame()) {
		ticks.clear();
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer) {
		ticks.clear();
		return;
	}

	if (!IsParticipatingTeam(*localPlayer->Team())) {
		ticks.clear();
		return;
	}

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity() || !IsParticipatingTeam(*player->Team())) {
			ticks[i].clear();
			continue;
		}

		if (!(friendlyFire || player->IsEnemy(localPlayer))) {
			ticks[i].clear();
			continue;
		}

		if (!ticks.contains(i))
			ticks[i] = {};

		const float currentSimulationTime = *player->SimulationTime();
		if (!ticks[i].empty()) {
			if (ticks[i].back().simulationTime == currentSimulationTime)
				continue; // We don't have a new position yet
			ticks[i].erase(std::remove_if(ticks[i].begin(), ticks[i].end(), [&](const auto& tick) {
				return !IsTickValid(tick);
			}),
				ticks[i].end());
		}

		Tick tick{};
		tick.simulationTime = currentSimulationTime;
		tick.tickCount = Memory::globalVars->tickcount;
		tick.origin = *player->Origin();
		memcpy(tick.boneMatrix, player->SetupBones(), sizeof(Matrix3x4[MAXSTUDIOBONES]));

		ticks[i].push_back(tick);
	}
}

void Features::Semirage::Backtrack::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !visualize)
		return;

	for (const auto& pair : ticks) {
		for (const auto& tick : pair.second) {
			ImVec2 screenOrigin;
			Features::Visuals::Esp::WorldToScreen(Hooks::Game::FrameStageNotify::worldToScreenMatrix, tick.origin, screenOrigin);
			drawList->AddCircleFilled(screenOrigin, 5.0f, ImGuiColors::white);
			ImVec2 screenHead;
			Features::Visuals::Esp::WorldToScreen(Hooks::Game::FrameStageNotify::worldToScreenMatrix, tick.boneMatrix[8].Origin(), screenHead);
			drawList->AddCircleFilled(screenHead, 5.0f, ImGuiColors::red);
		}
	}
}

void Features::Semirage::Backtrack::SetupGUI()
{
#ifndef MENUPREVIEW
	if (!ConVarStorage::cl_lagcompensation()->GetBool() || !ConVarStorage::sv_unlag()->GetBool())
		ImGui::TextColored(ImGuiColors::yellow, xorstr_("Warning: Judging by convars, lag compensation is disabled."));
#endif
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Scale"), &scale, 0.0f, 1.0f, xorstr_("%.2f"));
	ImGui::Checkbox(xorstr_("Account for outgoing ping"), &accountForOutgoingPing);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);
	ImGui::Checkbox(xorstr_("Visualize"), &visualize);

	ImGui::Separator();

	ImGui::Text(xorstr_("You are backtracking up to a maximum of %d ms"),
#ifndef MENUPREVIEW
		(int)(ConVarStorage::sv_maxunlag()->GetFloat() * scale * 1000 /*s to ms*/)
#else
		0
#endif
	);
}

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Backtrack::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Scale"), scale)
SERIALIZED_TYPE(xorstr_("Account for outgoing ping"), accountForOutgoingPing)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)
SERIALIZED_TYPE(xorstr_("Visualize"), visualize)
END_SERIALIZED_STRUCT
