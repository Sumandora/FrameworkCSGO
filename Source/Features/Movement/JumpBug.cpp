// https://github.com/seksea/gamesneeze/blob/ff795abb667979ea649973d4c80291b4071833a9/src/core/features/movement.cpp#L53

#include "JumpBug.hpp"

#include "imgui.h"

#include "../General/EnginePrediction.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../Interfaces.hpp"

static bool performing = false;

void JumpBug::CreateMove(CUserCmd* cmd)
{
	if (!enabled || (input.IsSet() && !input.IsActive())) {
		return;
	}

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (performing) {
		cmd->buttons |= IN_JUMP; // Cancel the bhop out
		performing = false;
		return;
	}

	const int realFlags = enginePrediction.prePredictionFlags;
	const int predFlags = *localPlayer->Flags();

	const bool isOnGround = realFlags & FL_ONGROUND || realFlags & FL_PARTIALGROUND;
	const bool willBeOnGround = predFlags & FL_ONGROUND || predFlags & FL_PARTIALGROUND;

	if (!isOnGround) {
		if (willBeOnGround) {
			cmd->buttons |= IN_DUCK;
			cmd->buttons &= ~IN_JUMP;

			performing = true;
		} else if (preDuck && localPlayer->Velocity()->z < 0.0 /* falling */) {
			// It is important when we release Duck, not when we press it,
			// so lets press it before we have to, to make it look more legit
			cmd->buttons |= IN_DUCK;
		}
	}
}

void JumpBug::SetupGUI()
{
	enginePrediction.ImGuiWarning();
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::InputSelector("Input (%s)", input);
	ImGui::Checkbox("Pre duck", &preDuck);
}

SCOPED_SERIALIZER(JumpBug)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_STRUCT("Input", input);
	SERIALIZE("Pre duck", preDuck);
}
