#include "Aimbot.hpp"

#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <algorithm>
#include <cmath>

bool  Features::Legit::Aimbot::enabled	  = false;
float Features::Legit::Aimbot::fov		  = 3.0f;
float Features::Legit::Aimbot::smoothness = 4.0f;
int	  Features::Legit::Aimbot::clamp	  = 1;

void  Features::Legit::Aimbot::PollEvent(SDL_Event* event) {
	 if (!enabled)
		 return;

	 if (event->type != SDL_MOUSEMOTION)
		 return;

	 if (!Interfaces::engine->IsInGame())
		 return;

	 int		  localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	 CBasePlayer* localPlayer	   = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	 if (!localPlayer)
		 return;

	 TeamID localTeam = *localPlayer->Team();
	 if (localTeam == TeamID::TEAM_UNASSIGNED || localTeam == TeamID::TEAM_SPECTATOR)
		 return;

	 Vector playerEye = localPlayer->GetEyePosition();

	 Vector viewAngles;
	 Interfaces::engine->GetViewAngles(&viewAngles);

	 CBasePlayer*		target = nullptr;
	 float				bestDistance;
	 Vector				bestRotation;

	 CTraceFilterEntity filter(localPlayer);

	 // The first object is always the WorldObj
	 for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		 CBasePlayer* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		 if (!player || player == localPlayer || player->GetDormant() || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity() || *player->Team() == localTeam)
			 continue;

		 Matrix3x4 boneMatrix[MAXSTUDIOBONES];
		 if (!player->SetupBones(boneMatrix))
			 continue;

		 Vector head  = boneMatrix[8].Origin();

		 Trace	trace = Utils::TraceRay(playerEye, head, &filter);

		 if (trace.m_pEnt != player)
			 continue; // The enemy is behind something...

		 Vector rotation = Utils::CalculateView(playerEye, head);
		 rotation		 -= *localPlayer->AimPunchAngle() * 2;
		 rotation		 -= viewAngles;
		 rotation		 = rotation.Wrap();

		 float delta	 = rotation.Length();
		 if (!target || bestDistance > delta) {
			 target		  = player;
			 bestDistance = delta;
			 bestRotation = rotation;
		 }
	 }

	 if (!target)
		 return;

	 if (bestRotation.Length() > fov)
		 return;

	 bestRotation  /= smoothness;

	 Vector before = Vector(event->motion.xrel, event->motion.yrel, 0);
	 Vector goal   = Vector(-round(bestRotation.y), round(bestRotation.x), 0);

	 float	dir	   = before.Normalized().Dot(goal.Normalized());
	 if (dir < 0)
		 return; // We are trying to aim away

	 event->motion.xrel += std::clamp(static_cast<int>(goal.x), -clamp, clamp);
	 event->motion.yrel += std::clamp(static_cast<int>(goal.y), -clamp, clamp);
}

void Features::Legit::Aimbot::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitAimbot"), &enabled);
	ImGui::SliderFloat(xorstr_("FOV##LegitAimbot"), &fov, 0.0f, 10.0f, "%.2f");
	ImGui::SliderFloat(xorstr_("Smoothness##LegitAimbot"), &smoothness, 1.0f, 5.0f, "%.2f");
	ImGui::SliderInt(xorstr_("Clamp##LegitAimbot"), &clamp, 1, 5);
}
