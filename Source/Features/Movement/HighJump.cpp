#include "HighJump.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../GUI/Elements/Keybind.hpp"

bool Features::Movement::HighJump::enabled = false;
int Features::Movement::HighJump::input = 0;

bool performing = false;

void Features::Movement::HighJump::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false)) {
		performing = false;
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer) {
		performing = false;
		return;
	}

	int flags = *localPlayer->Flags();

	cmd->buttons |= IN_DUCK;
	if (flags & FL_ONGROUND) {
		if (performing) {
			cmd->buttons |= IN_JUMP;
			performing = false;
		} else
			performing = true;
	}
}

void Features::Movement::HighJump::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
}
