#include "../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GameCache.hpp"

#include <vector>

bool Features::Visuals::NoPunch::hideViewPunch = false;
bool Features::Visuals::NoPunch::hideAimPunch = false;

static Vector lastViewPunch;
static Vector lastAimPunch;

void Features::Visuals::NoPunch::HidePunch() {
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if(!localPlayer || *localPlayer->LifeState() != LIFE_ALIVE)
		return;

	if(hideViewPunch)
		*localPlayer->ViewPunchAngle() = {};

	if(hideAimPunch)
		*localPlayer->AimPunchAngle() = {};

	lastViewPunch = *localPlayer->ViewPunchAngle();
	lastAimPunch = *localPlayer->AimPunchAngle();
}

void Features::Visuals::NoPunch::RestorePunch() {
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if(!localPlayer || *localPlayer->LifeState() != LIFE_ALIVE)
		return;

	if(hideViewPunch)
		*localPlayer->ViewPunchAngle() = lastViewPunch;

	if(hideAimPunch)
		*localPlayer->AimPunchAngle() = lastAimPunch;
}

void Features::Visuals::NoPunch::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Hide view punch"), &hideViewPunch);
	ImGui::Checkbox(xorstr_("Hide aim punch"), &hideAimPunch);
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::NoPunch::Serializer, xorstr_("No punch"))
SERIALIZED_TYPE(xorstr_("Hide view punch"), hideViewPunch);
SERIALIZED_TYPE(xorstr_("Hide aim punch"), hideAimPunch);
END_SERIALIZED_STRUCT
