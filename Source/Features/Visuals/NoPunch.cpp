#include "NoPunch.hpp"

#include "../../Interfaces.hpp"
#include "imgui.h"
#include <vector>

static Vector lastViewPunch;
static Vector lastAimPunch;

void NoPunch::HidePunch()
{
	if ((!hideViewPunch && !hideAimPunch) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (hideViewPunch)
		*localPlayer->ViewPunchAngle() = {};

	if (hideAimPunch)
		*localPlayer->AimPunchAngle() = {};

	lastViewPunch = *localPlayer->ViewPunchAngle();
	lastAimPunch = *localPlayer->AimPunchAngle();
}

void NoPunch::RestorePunch()
{
	if ((!hideViewPunch && !hideAimPunch) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (hideViewPunch)
		*localPlayer->ViewPunchAngle() = lastViewPunch;

	if (hideAimPunch)
		*localPlayer->AimPunchAngle() = lastAimPunch;
}

void NoPunch::SetupGUI()
{
	ImGui::Checkbox("Hide view punch", &hideViewPunch);
	ImGui::Checkbox("Hide aim punch", &hideAimPunch);
}

SCOPED_SERIALIZER(NoPunch)
{
	SERIALIZE("Hide view punch", hideViewPunch);
	SERIALIZE("Hide aim punch", hideAimPunch);
}
