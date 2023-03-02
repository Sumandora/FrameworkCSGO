#include "Backtrack.hpp"
#include <deque>
#include <map>
#include <vector>

#include "../../GUI/EventLog.hpp"
#include "../../GameCache.hpp"
#include "../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"
#include "../../Interfaces.hpp"
#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../Legit/ESP.hpp"

bool Features::Semirage::Backtrack::enabled = false;
float Features::Semirage::Backtrack::timeDelta = 0.2f;

struct Tick {
	float simulationTime{};
	int tickCount{};
	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
};

std::map<int, std::vector<Tick>> ticks;
int currentTickCount;

void Features::Semirage::Backtrack::CreateMove(CUserCmd* cmd) {
	currentTickCount = cmd->tick_count;
	if(!enabled || !(cmd->buttons & IN_ATTACK))
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	TeamID localTeam = *localPlayer->Team();
	if (localTeam == TeamID::TEAM_UNASSIGNED || localTeam == TeamID::TEAM_SPECTATOR)
		return;

	float bestDistance = FLT_MAX;
	Tick bestTick;

	for (const auto& pair : ticks) {
		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(pair.first));
		if (!player || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity() || *player->Team() == localTeam) {
			continue;
		}
		for (const auto& tick : pair.second) {
			Vector requiredView = Utils::CalculateView(localPlayer->GetEyePosition(), tick.boneMatrix[8].Origin());
			requiredView -= *localPlayer->AimPunchAngle() * 2;
			requiredView -= cmd->viewangles;
			requiredView.Wrap();

			float delta = requiredView.Length();

			if(bestDistance > delta) {
				bestDistance = delta;
				bestTick = tick;
			}
		}
	}

	if(bestDistance < 5.0f) {
		//Gui::EventLog::CreateReport("Trying to backtrack %d ticks", bestTick.tickCount - cmd->tick_count);
		cmd->tick_count = bestTick.tickCount;
	}
}

void Features::Semirage::Backtrack::FrameStageNotify() {
	if (!enabled || !Interfaces::engine->IsInGame()) {
		ticks.clear();
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	TeamID localTeam = *localPlayer->Team();
	if (localTeam == TeamID::TEAM_UNASSIGNED || localTeam == TeamID::TEAM_SPECTATOR)
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		if(!ticks.contains(i))
			ticks[i] = {};

		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == localPlayer || player->GetDormant() || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity() || *player->Team() == localTeam) {
			ticks[i].clear();
			continue;
		}

		float currentSimulationTime = *player->SimulationTime();
		if(!ticks[i].empty()) {
			if(ticks[i].back().simulationTime == currentSimulationTime)
				continue; // We don't have a new position yet
			ticks[i].erase(std::remove_if(ticks[i].begin(), ticks[i].end(), [&](const auto& tick) {
				return currentSimulationTime - tick.simulationTime > timeDelta;
			}), ticks[i].end());
		}

		Tick tick;
		tick.simulationTime = currentSimulationTime;
		tick.tickCount = currentTickCount;
		player->SetupBones(tick.boneMatrix);

		ticks[i].push_back(tick);
	}
}

void Features::Semirage::Backtrack::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##SemirageBacktrack"), &enabled);
	ImGui::SliderFloat(xorstr_("Time delta##SemirageBacktrack"), &timeDelta, 0.0f, 0.2f, "%.2f");
}