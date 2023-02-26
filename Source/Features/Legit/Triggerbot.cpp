#include "Triggerbot.hpp"

#include "xorstr.hpp"
#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/Raytrace.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

bool	Features::Legit::Triggerbot::enabled	= false;
int		Features::Legit::Triggerbot::input		= ImGuiKey_V;

void Features::Legit::Triggerbot::CreateMove(CUserCmd* cmd) {
	if(!enabled || !IsInputDown(input))
		return;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	CBasePlayer* localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if(!localPlayer)
		return;

	TeamID localTeam = *localPlayer->Team();
	if(	localTeam == TeamID::TEAM_UNASSIGNED ||
		localTeam == TeamID::TEAM_SPECTATOR)
		return;

	Vector playerEye = localPlayer->GetEyePosition();
	Vector viewangles = Vector(cmd->viewangles);

	viewangles += *localPlayer->AimPunchAngle();

	Vector forward;
	Utils::AngleVectors(viewangles, &forward);

	forward = playerEye + (forward * 4096.0f);

	CTraceFilterEntity filter(localPlayer);

	Trace trace = Utils::TraceRay(playerEye, forward, &filter);

	CBaseEntity* entity = trace.m_pEnt;
	if(	!entity ||
		 entity == localPlayer ||
		!entity->IsPlayer() ||
		 entity->GetDormant()
	) return;

	CBasePlayer* player = reinterpret_cast<CBasePlayer*>(entity);
	if( *player->LifeState() != LIFE_ALIVE ||
		*player->GunGameImmunity() ||
		*player->Team() == localTeam)
		return;

	cmd->buttons |= IN_ATTACK;
}

void Features::Legit::Triggerbot::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitTriggerbot"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)##LegitTriggerbot"), input);
}
