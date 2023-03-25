#include "../Semirage.hpp"

#include "imgui.h"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../Hooks/CreateMove/CreateMoveHook.hpp"
#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <algorithm>

static bool enabled = false;
static bool onlyWhenShooting = false; // TODO Separate key
static float fov = 3.0f;
static float aimSpeed = 0.2f;
static int maximalFlashAmount = 255;
static bool dontAimThroughSmoke = false;
static bool silent = false;
static float snapBack = 0.1f;
static bool friendlyFire = false;

bool wasFaked = false;

bool Features::Semirage::Aimbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return false;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return false;

	// TODO Add Alive check

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return false;

	Vector viewAngles;
	if (silent && wasFaked && Hooks::CreateMove::lastCmd)
		viewAngles = Hooks::CreateMove::lastCmd->viewangles;
	else
		viewAngles = Vector(cmd->viewangles);

	CBasePlayer* target = nullptr;
	Vector bestRotation;
	float bestDistance {};

	// For compatibility’s sake, play it off like we didn't find a target
	if ((cmd->buttons & IN_ATTACK || !onlyWhenShooting) && *reinterpret_cast<float*>(reinterpret_cast<char*>(localPlayer->FlashMaxAlpha()) - 0x8) <= (float)maximalFlashAmount) {
		CTraceFilterEntity filter(localPlayer);

		// The first object is always the WorldObj
		for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
			auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
			if (!player || player == localPlayer || player->GetDormant() || *player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity())
				continue;

			if (!IsParticipatingTeam(*player->Team()))
				continue;

			if (!(friendlyFire || player->IsEnemy()))
				continue;

			const Vector playerEye = localPlayer->GetEyePosition();

			Matrix3x4 boneMatrix[MAXSTUDIOBONES];
			if (!player->SetupBones(boneMatrix))
				continue;

			const Vector head = boneMatrix[8].Origin();

			if (dontAimThroughSmoke && Memory::LineGoesThroughSmoke(playerEye, head, 1))
				continue;

			const Trace trace = Utils::TraceRay(playerEye, head, &filter);

			if (trace.m_pEnt != player)
				continue; // The enemy is behind something...

			Vector rotation = Utils::CalculateView(playerEye, head);
			rotation -= *localPlayer->AimPunchAngle() * ConVarStorage::weapon_recoil_scale->GetFloat();
			const float delta = (rotation - cmd->viewangles).Wrap().Length(); // Use the real angle for the fov check
			rotation -= viewAngles;
			rotation.Wrap();

			if (!target || bestDistance > delta) {
				target = player;
				bestDistance = delta;
				bestRotation = rotation;
			}
		}
	}

	if (!target || bestDistance > fov) {
		if (silent && wasFaked) {
			Vector delta = cmd->viewangles - viewAngles;
			delta.Wrap();
			if (delta.Length() < snapBack) {
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
	if (!silent)
		Interfaces::engine->SetViewAngles(&cmd->viewangles);

	wasFaked = true;
	return silent;
}

void Features::Semirage::Aimbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::Checkbox(xorstr_("Only when shooting"), &onlyWhenShooting);
	ImGui::SliderFloat(xorstr_("FOV"), &fov, 0.0f, 10.0f, "%.2f");
	ImGui::SliderFloat(xorstr_("Aim speed"), &aimSpeed, 0.0f, 1.0f, "%.2f");
	ImGui::SliderInt(xorstr_("Maximal flash amount"), &maximalFlashAmount, 0, 255);
	ImGui::Checkbox(xorstr_("Don't aim through smoke"), &dontAimThroughSmoke);
	ImGui::Checkbox(xorstr_("Silent"), &silent);
	if (silent) {
		ImGui::SliderFloat(xorstr_("Snapback"), &snapBack, 0.0f, 1.0f, "%.2f");
		ImGui::HelpMarker(xorstr_("Unlike other cheats, silent aim is smoothed out. At some point, we have to combine the rotations again, this setting tells Framework when to do that"));
	}
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);
}

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Aimbot::Serializer, xorstr_("Semirage Aimbot"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Only when shooting"), onlyWhenShooting)
SERIALIZED_TYPE(xorstr_("FOV"), fov)
SERIALIZED_TYPE(xorstr_("Maximal flash amount"), maximalFlashAmount)
SERIALIZED_TYPE(xorstr_("Aim speed"), aimSpeed)
SERIALIZED_TYPE(xorstr_("Don't aim through smoke"), dontAimThroughSmoke)
SERIALIZED_TYPE(xorstr_("Silent"), silent)
SERIALIZED_TYPE(xorstr_("Snapback"), snapBack)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)
END_SERIALIZED_STRUCT