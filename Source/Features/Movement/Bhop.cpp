#include "Movement.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "imgui.h"

#include "../../Hooks/Game/GameFunctions.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../Interfaces.hpp"
#include "../../SDK/MoveType.hpp"
#include <cstdlib>

static bool enabled = false;
static float hitChance = 1.0f;
static float inAirDistortion = 0.0f;
static int minimumReleaseDelay = 0;
static int maximumRelaseDelay = 0;
static bool onlyWhenFalling = false;

void Features::Movement::Bhop::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (onlyWhenFalling && localPlayer->Velocity()->z > 0.0)
		return;

	const int flags = *localPlayer->Flags();
	const int tick = cmd->tick_count;

	static int lastJump = 0;
	static int nextRandomDelay = 0;

	if (cmd->buttons & IN_JUMP) {
		float randomValue = rand() / (float)RAND_MAX;
		if (!(flags & FL_ONGROUND)) {
			if (randomValue > inAirDistortion && tick > lastJump + nextRandomDelay)
				cmd->buttons &= ~IN_JUMP;
		} else {
			if (randomValue > hitChance || Hooks::Game::CreateMove::lastCmd.buttons & IN_JUMP /* If we held the button last tick, then we need to stop holding it now; TODO Fix this using prediction */) {
				cmd->buttons &= ~IN_JUMP;
			}
		}
	}

	if (flags & FL_ONGROUND && cmd->buttons & IN_JUMP) {
		lastJump = tick; // We just did a jump

		float randomValue = rand() / (float)RAND_MAX;
		nextRandomDelay = minimumReleaseDelay + (maximumRelaseDelay - minimumReleaseDelay) * randomValue;
	}
}

void Features::Movement::Bhop::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Hit chance"), &hitChance, 0, 1, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("In air distortion"), &inAirDistortion, 0, 1, xorstr_("%.2f"));
	ImGui::SliderInt(xorstr_("Minimum release delay"), &minimumReleaseDelay, 0, 10);
	ImGui::SliderInt(xorstr_("Maximum release delay"), &maximumRelaseDelay, 0, 10);
	ImGui::Checkbox(xorstr_("Only when falling"), &onlyWhenFalling);
	ImGui::HelpMarker(xorstr_("This is useful when wearing an exojump suit"));
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::Bhop::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Hit chance"), hitChance)
SERIALIZED_TYPE(xorstr_("In air distortion"), inAirDistortion)
SERIALIZED_TYPE(xorstr_("Minimum release delay"), minimumReleaseDelay)
SERIALIZED_TYPE(xorstr_("Maximum release delay"), maximumRelaseDelay)
SERIALIZED_TYPE(xorstr_("Only when falling"), onlyWhenFalling)
END_SERIALIZED_STRUCT
