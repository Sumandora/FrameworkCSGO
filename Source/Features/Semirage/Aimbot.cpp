#include "../Semirage.hpp"

#include "imgui.h"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../Hooks/CreateMove/CreateMoveHook.hpp"
#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include <algorithm>
static bool enabled = false;

struct SemirageAimbotWeaponConfig {
	bool onlyWhenShooting = false; // TODO Separate key
	float fov = 3.0f;
	float aimSpeed = 0.2f;
	bool silent = false;
	float snapBack = 0.1f;

	DECLARE_SERIALIZER(Serializer);
};
void WeaponGUI(SemirageAimbotWeaponConfig& weaponConfig);

static WeaponConfigurator<SemirageAimbotWeaponConfig> weaponConfigurator(WeaponGUI);

static int maximalFlashAmount = 255;
static bool dontAimThroughSmoke = false;
static bool friendlyFire = false;

bool wasFaked = false;

bool Features::Semirage::Aimbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return false;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return false;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return false;

	auto* combatWeapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if(!combatWeapon)
		return false;

	if(!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return false;

	SemirageAimbotWeaponConfig* weaponConfig = weaponConfigurator.getConfig(*combatWeapon->WeaponDefinitionIndex());
	if(!weaponConfig)
		return false;

	Vector viewAngles;
	if (weaponConfig->silent && wasFaked && Hooks::CreateMove::lastCmd)
		viewAngles = Hooks::CreateMove::lastCmd->viewangles;
	else
		viewAngles = Vector(cmd->viewangles);

	CBasePlayer* target = nullptr;
	Vector bestRotation;
	float bestDistance {};

	// For compatibility’s sake, play it off like we didn't find a target
	if ((cmd->buttons & IN_ATTACK || !weaponConfig->onlyWhenShooting) && localPlayer->GetFlashAlpha() <= (float)maximalFlashAmount) {
		CTraceFilterEntity filter(localPlayer);

		// The first object is always the WorldObj
		for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
			auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
			if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
				continue;

			if (!IsParticipatingTeam(*player->Team()))
				continue;

			if (!(friendlyFire || player->IsEnemy(localPlayer)))
				continue;

			const Vector playerEye = localPlayer->GetEyePosition();

			Matrix3x4* boneMatrix = player->SetupBones();

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

	if (!target || bestDistance > weaponConfig->fov) {
		if (weaponConfig->silent && wasFaked) {
			Vector delta = cmd->viewangles - viewAngles;
			delta.Wrap();
			if (delta.Length() < weaponConfig->snapBack) {
				// At this point the difference is so small, that it no longer matters
				wasFaked = false;
				return false;
			}
			delta *= weaponConfig->aimSpeed;
			cmd->viewangles = viewAngles + delta;
			cmd->viewangles.Wrap();
			wasFaked = true;
			return true;
		}
		wasFaked = false;
		return false;
	}

	bestRotation *= weaponConfig->aimSpeed;
	cmd->viewangles = viewAngles + bestRotation;
	cmd->viewangles.Wrap();
	if (!weaponConfig->silent)
		Interfaces::engine->SetViewAngles(&cmd->viewangles);

	wasFaked = true;
	return weaponConfig->silent;
}

void WeaponGUI(SemirageAimbotWeaponConfig& weaponConfig) {
	ImGui::Checkbox(xorstr_("Only when shooting"), &weaponConfig.onlyWhenShooting);
	ImGui::SliderFloat(xorstr_("FOV"), &weaponConfig.fov, 0.0f, 10.0f, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("Aim speed"), &weaponConfig.aimSpeed, 0.0f, 1.0f, xorstr_("%.2f"));
	ImGui::Checkbox(xorstr_("Silent"), &weaponConfig.silent);
	if (weaponConfig.silent) {
		ImGui::SliderFloat(xorstr_("Snapback"), &weaponConfig.snapBack, 0.0f, 1.0f, xorstr_("%.2f"));
		ImGui::HelpMarker(xorstr_("Unlike other cheats, silent aim is smoothed out. At some point, we have to combine the rotations again, this setting tells Framework when to do that"));
	}
}

void Features::Semirage::Aimbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);

	ImGui::SliderInt(xorstr_("Maximal flash amount"), &maximalFlashAmount, 0, 255);
	ImGui::Checkbox(xorstr_("Don't aim through smoke"), &dontAimThroughSmoke);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);

	weaponConfigurator.SetupGUI();
}

BEGIN_SERIALIZED_STRUCT(SemirageAimbotWeaponConfig::Serializer)
SERIALIZED_TYPE(xorstr_("Only when shooting"), onlyWhenShooting)
SERIALIZED_TYPE(xorstr_("FOV"), fov)
SERIALIZED_TYPE(xorstr_("Aim speed"), aimSpeed)
SERIALIZED_TYPE(xorstr_("Silent"), silent)
SERIALIZED_TYPE(xorstr_("Snapback"), snapBack)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Aimbot::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Maximal flash amount"), maximalFlashAmount)
SERIALIZED_TYPE(xorstr_("Don't aim through smoke"), dontAimThroughSmoke)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)

SERIALIZED_STRUCTURE(xorstr_("Weapons"), weaponConfigurator)
END_SERIALIZED_STRUCT