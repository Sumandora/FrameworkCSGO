#include "Aimbot.hpp"

#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../Hooks/CreateMove/CreateMoveHook.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/Raytrace.hpp"
#include "../../SDK/Definitions/InputFlags.hpp"

#include <algorithm>

bool	Features::Semirage::Aimbot::enabled				= false;
bool	Features::Semirage::Aimbot::onlyWhenShooting	= false;
float	Features::Semirage::Aimbot::fov					= 3.0f;
float	Features::Semirage::Aimbot::aimSpeed			= 0.2f;
bool	Features::Semirage::Aimbot::silent				= false;
float	Features::Semirage::Aimbot::snapBack			= 0.1f;

bool wasFaked = false;

bool Features::Semirage::Aimbot::CreateMove(CUserCmd *cmd) {
	if(!enabled || !Interfaces::engine->IsInGame())
		return false;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	CBasePlayer* localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if(!localPlayer)
		return false;

	TeamID localTeam = *localPlayer->Team();
	if( localTeam == TeamID::TEAM_UNASSIGNED ||
		localTeam == TeamID::TEAM_SPECTATOR)
		return false;

	Vector playerEye = localPlayer->GetEyePosition();

	Vector viewAngles;
	if(silent && wasFaked && Hooks::CreateMove::lastCmd)
		viewAngles = Hooks::CreateMove::lastCmd->viewangles;
	else
		viewAngles = Vector(cmd->viewangles);

	CBasePlayer* target = nullptr;
	Vector bestRotation;
	float bestDistance;

	// For compatibility’s sake, play it off like we didn't find a target
	if(cmd->buttons & IN_ATTACK || !onlyWhenShooting) {
		CTraceFilterEntity filter(localPlayer);

		// The first object is always the WorldObj
		for(int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
			CBasePlayer* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
			if( !player ||
				player == localPlayer ||
				player->GetDormant() ||
				*player->LifeState() != LIFE_ALIVE ||
				*player->GunGameImmunity() ||
				*player->Team() == localTeam)
				continue;

			Matrix3x4 boneMatrix[MAXSTUDIOBONES];
			if(!player->SetupBones(boneMatrix))
				continue;

			Vector head = boneMatrix[8].Origin();

			Trace trace = Utils::TraceRay(playerEye, head, &filter);

			if(trace.m_pEnt != player)
				continue; // The enemy is behind something...

			Vector rotation = Utils::CalculateView(playerEye, head);
			rotation -= *localPlayer->AimPunchAngle() * 2;
			float delta = (rotation - cmd->viewangles).Wrap().Length(); // Use the real angle for the fov check
			rotation -= viewAngles;
			rotation.Wrap();

			if(!target || bestDistance > delta) {
				target = player;
				bestDistance = delta;
				bestRotation = rotation;
			}
		}
	}

	if(!target || bestDistance > fov) {
		if(silent && wasFaked) {
			Vector delta = cmd->viewangles - viewAngles;
			delta.Wrap();
			if(delta.Length() < snapBack) {
				// At this point the difference is so small, that it no longer matters
				wasFaked = false;
				return false;
			}
			delta *= aimSpeed;
			cmd->viewangles = viewAngles + delta;
			cmd->viewangles.Wrap();
			wasFaked = true;
			return true;
		}
		wasFaked = false;
		return false;
	}

	bestRotation *= aimSpeed;
	cmd->viewangles = viewAngles + bestRotation;
	cmd->viewangles.Wrap();
	if(!silent)
		Interfaces::engine->SetViewAngles(&cmd->viewangles);

	wasFaked = true;
	return silent;
}

void Features::Semirage::Aimbot::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##SemirageAimbot"), &enabled);
	ImGui::Checkbox(xorstr_("Only when shooting##SemirageAimbot"), &onlyWhenShooting);
	ImGui::SliderFloat(xorstr_("FOV##SemirageAimbot"), &fov, 0.0f, 10.0f, "%.2f");
	ImGui::SliderFloat(xorstr_("Aim speed##SemirageAimbot"), &aimSpeed, 0.0f, 1.0f, "%.2f");
	ImGui::Checkbox(xorstr_("Silent##SemirageAimbot"), &silent);
	if(silent) {
		ImGui::SliderFloat(xorstr_("Snapback##SemirageAimbot"), &snapBack, 0.0f, 1.0f, "%.2f");
		if(ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", xorstr_("Unlike other cheats, silent aim is smoothed out. At some point, we have to combine the rotations again, this setting tells Framework when to do that"));
	}
}