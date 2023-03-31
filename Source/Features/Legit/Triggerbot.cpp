#include "../Legit.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Interfaces.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../GameCache.hpp"
#include "../../GUI/Elements/Keybind.hpp"

static bool enabled = false;
static int input = 0;
static bool secondaryFireWithR8Revolver = true;
static bool friendlyFire = false;
// TODO Delay

void Features::Legit::Triggerbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false))
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));

	if (!weapon)
		return;

	const bool secondaryFire = *weapon->WeaponDefinitionIndex() == WeaponID::WEAPON_REVOLVER && secondaryFireWithR8Revolver;

	if ((!secondaryFire ? *weapon->NextPrimaryAttack() : *weapon->NextSecondaryAttack()) > Memory::globalVars->curtime)
		return;

	const Vector playerEye = localPlayer->GetEyePosition();
	auto viewangles = Vector(cmd->viewangles);

	viewangles += *localPlayer->AimPunchAngle() * ConVarStorage::weapon_recoil_scale->GetFloat() / 2.0f;

	Vector forward;
	Utils::AngleVectors(viewangles, &forward);

	forward = playerEye + (forward * 4096.0f);

	CTraceFilterEntity filter(localPlayer);

	Trace trace = Utils::TraceRay(playerEye, forward, &filter);

	CBaseEntity* entity = trace.m_pEnt;
	if (!entity || !entity->IsPlayer() || entity->GetDormant())
		return;

	auto* player = reinterpret_cast<CBasePlayer*>(entity);
	if (!player->IsAlive() || *player->GunGameImmunity())
		return;

	TeamID team = *player->Team();

	if (!IsParticipatingTeam(team))
		return;

	if (!(friendlyFire || player->IsEnemy(localPlayer)))
		return;

	if (secondaryFire)
		cmd->buttons |= IN_ATTACK2;
	else
		cmd->buttons |= IN_ATTACK;
}

void Features::Legit::Triggerbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
	ImGui::Checkbox(xorstr_("Secondary fire with R8 Revolver"), &secondaryFireWithR8Revolver);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);
}

BEGIN_SERIALIZED_STRUCT(Features::Legit::Triggerbot::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
SERIALIZED_TYPE(xorstr_("Secondary fire with R8 Revolver"), secondaryFireWithR8Revolver)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)
END_SERIALIZED_STRUCT
