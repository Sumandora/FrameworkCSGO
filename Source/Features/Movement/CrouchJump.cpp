#include "../Movement.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../SDK/MoveType.hpp"
#include "../../GUI/Elements/Keybind.hpp"


bool Features::Movement::CrouchJump::enabled = false;
int Features::Movement::CrouchJump::input = 0;

static bool performing = false;

void Features::Movement::CrouchJump::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, false)) {
		performing = false;
		return;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || *localPlayer->LifeState() != LIFE_ALIVE) {
		performing = false;
		return;
	}

	if(localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
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

BEGIN_SERIALIZED_STRUCT(Features::Movement::CrouchJump::Serializer, xorstr_("Crouch jump"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
END_SERIALIZED_STRUCT
