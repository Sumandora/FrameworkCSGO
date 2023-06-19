#include "Triggerbot.hpp"

#include "../../Interfaces.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../GUI/Elements/Keybind.hpp"

void Triggerbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return;

	auto* weapon = static_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));

	if (!weapon)
		return;

	const bool secondaryFire = *weapon->WeaponDefinitionIndex() == WeaponID::WEAPON_REVOLVER && secondaryFireWithR8Revolver;

	if ((!secondaryFire ? *weapon->NextPrimaryAttack() : *weapon->NextSecondaryAttack()) > Memory::globalVars->curtime)
		return;

	const Vector playerEye = localPlayer->GetEyePosition();
	Vector viewangles = cmd->viewangles;

	viewangles += *localPlayer->AimPunchAngle();

	Vector forward;
	Utils::AngleVectors(viewangles, &forward);

	forward = playerEye + (forward * 4096.0f);

	CTraceFilterEntity filter(localPlayer);

	const Trace trace = Utils::TraceRay(playerEye, forward, &filter);

	if (dontShootThroughSmoke && Memory::LineGoesThroughSmoke(playerEye, trace.endpos, 1))
		return;

	CBaseEntity* entity = trace.m_pEnt;
	if (!entity || !entity->IsPlayer() || entity->GetDormant())
		return;

	auto* player = static_cast<CBasePlayer*>(entity);
	if (!player->IsAlive() || *player->GunGameImmunity())
		return;

	const TeamID team = *player->Team();

	if (!IsParticipatingTeam(team))
		return;

	if (!(friendlyFire || player->IsEnemy(localPlayer)))
		return;

	if (secondaryFire)
		cmd->buttons |= IN_ATTACK2;
	else
		cmd->buttons |= IN_ATTACK;
}

void Triggerbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
	ImGui::Checkbox(xorstr_("Secondary fire with R8 Revolver"), &secondaryFireWithR8Revolver);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);
	ImGui::SliderInt(xorstr_("Maximal flash amount"), &maximalFlashAmount, 0, 255);
	ImGui::Checkbox(xorstr_("Don't shoot through smoke"), &dontShootThroughSmoke);
}

SCOPED_SERIALIZER(Triggerbot)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE(xorstr_("Input"), input);
	SERIALIZE(xorstr_("Secondary fire with R8 Revolver"), secondaryFireWithR8Revolver);
	SERIALIZE(xorstr_("Friendly fire"), friendlyFire);
	SERIALIZE(xorstr_("Maximal flash amount"), maximalFlashAmount);
	SERIALIZE(xorstr_("Don't shoot through smoke"), dontShootThroughSmoke);
}
