#include "../Visuals.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"
#include "imgui.h"
#include "xorstr.hpp"

#include <vector>

static bool hideViewPunch = false;
static bool hideAimPunch = false;

static Vector lastViewPunch;
static Vector lastAimPunch;

void Features::Visuals::NoPunch::HidePunch()
{
	if((!hideViewPunch && !hideAimPunch) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (hideViewPunch)
		*localPlayer->ViewPunchAngle() = {};

	if (hideAimPunch)
		*localPlayer->AimPunchAngle() = {};

	lastViewPunch = *localPlayer->ViewPunchAngle();
	lastAimPunch = *localPlayer->AimPunchAngle();
}

void Features::Visuals::NoPunch::RestorePunch()
{
	if((!hideViewPunch && !hideAimPunch) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (hideViewPunch)
		*localPlayer->ViewPunchAngle() = lastViewPunch;

	if (hideAimPunch)
		*localPlayer->AimPunchAngle() = lastAimPunch;
}

void Features::Visuals::NoPunch::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Hide view punch"), &hideViewPunch);
	ImGui::Checkbox(xorstr_("Hide aim punch"), &hideAimPunch);
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::NoPunch::Serializer)
SERIALIZED_TYPE(xorstr_("Hide view punch"), hideViewPunch)
SERIALIZED_TYPE(xorstr_("Hide aim punch"), hideAimPunch)
END_SERIALIZED_STRUCT
