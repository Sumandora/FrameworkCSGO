#include "Triggerbot.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../ConVarStorage.hpp"
#include "../../Interfaces.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../GUI/Elements/Keybind.hpp"

bool Features::Legit::Triggerbot::enabled = false;
int Features::Legit::Triggerbot::input = 0;

void Features::Legit::Triggerbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false))
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	TeamID localTeam = *localPlayer->Team();
	if (localTeam == TeamID::TEAM_UNASSIGNED || localTeam == TeamID::TEAM_SPECTATOR)
		return;

	Vector playerEye = localPlayer->GetEyePosition();
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

	auto player = reinterpret_cast<CBasePlayer*>(entity);
	if (*player->LifeState() != LIFE_ALIVE || *player->GunGameImmunity() || *player->Team() == localTeam)
		return;

	cmd->buttons |= IN_ATTACK;
}

void Features::Legit::Triggerbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
}

BEGIN_SERIALIZED_STRUCT(Features::Legit::Triggerbot::Serializer, xorstr_("Spectator list"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
END_SERIALIZED_STRUCT
