// https://github.com/seksea/gamesneeze/blob/ff795abb667979ea649973d4c80291b4071833a9/src/core/features/movement.cpp#L53

#include "../Movement.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../General.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../GUI/Elements/Keybind.hpp"

bool Features::Movement::JumpBug::enabled = false;
int Features::Movement::JumpBug::input = 0;
bool Features::Movement::JumpBug::preDuck = false;

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

	int realFlags = Features::General::EnginePrediction::prePredictionFlags;
	int predFlags = *localPlayer->Flags();

	bool isOnGround = realFlags & FL_ONGROUND || realFlags & FL_PARTIALGROUND;
	bool willBeOnGround = predFlags & FL_ONGROUND || predFlags & FL_PARTIALGROUND;

	if (!isOnGround) {
		if (willBeOnGround) {
			cmd->buttons |= IN_DUCK;
			cmd->buttons &= ~IN_JUMP;

			performing = true;
		} else if (preDuck && localPlayer->Velocity()->z < 0.0 /* falling */) {
			// It is important when we release Duck, not when we press it,
			// so lets press it before we have to to make it look more legit
			cmd->buttons |= IN_DUCK;
		}
	}
}

void Features::Movement::JumpBug::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
	ImGui::Checkbox(xorstr_("Pre duck"), &preDuck);
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::JumpBug::Serializer, xorstr_("High jump"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
SERIALIZED_TYPE(xorstr_("Pre duck"), preDuck)
END_SERIALIZED_STRUCT
