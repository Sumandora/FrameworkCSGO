// https://github.com/seksea/gamesneeze/blob/ff795abb667979ea649973d4c80291b4071833a9/src/core/features/movement.cpp#L53

#include "JumpBug.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../General/EnginePrediction.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../GUI/Elements/Keybind.hpp"

bool Features::Movement::JumpBug::enabled = false;
int Features::Movement::JumpBug::input = 0;

static bool performing = false;

void Features::Movement::JumpBug::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false)) {
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer) {
		return;
	}

	if (performing) {
		cmd->buttons |= IN_JUMP; // Cancel the bhop out
		performing = false;
		return;
	}

	int oldFlags = Features::General::EnginePrediction::prePredictionFlags;
	int flags = *localPlayer->Flags();

	if (!(oldFlags & FL_ONGROUND || oldFlags & FL_PARTIALGROUND) && (flags & FL_ONGROUND || flags & FL_PARTIALGROUND)) {
		cmd->buttons |= IN_DUCK;
		cmd->buttons &= ~IN_JUMP;

		performing = true;
	}
}

void Features::Movement::JumpBug::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::JumpBug::Serializer, xorstr_("High jump"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
END_SERIALIZED_STRUCT
