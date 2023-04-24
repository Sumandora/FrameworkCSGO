#include "../Movement.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../GUI/Elements/Keybind.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../SDK/MoveType.hpp"

static bool enabled = false;
static int input = ImGuiKey_None;

static bool performing = false;

void Features::Movement::CrouchJump::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false)) {
		performing = false;
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive()) {
		performing = false;
		return;
	}

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	const int flags = *localPlayer->Flags();

	cmd->buttons |= IN_DUCK;
	if (flags & FL_ONGROUND) {
		if (performing) {
			cmd->buttons |= IN_JUMP;
			performing = false;
		} else
			performing = true;
	}
}

void Features::Movement::CrouchJump::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::CrouchJump::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
END_SERIALIZED_STRUCT
