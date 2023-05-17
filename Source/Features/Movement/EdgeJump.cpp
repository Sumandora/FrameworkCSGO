// https://github.com/seksea/gamesneeze/blob/ff795abb667979ea649973d4c80291b4071833a9/src/core/features/movement.cpp#L53

#include "Movement.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../General/General.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../Interfaces.hpp"

static bool enabled = false;
static int input = ImGuiKey_None;

void Features::Movement::EdgeJump::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !IsInputDown(input, true)) {
		return;
	}

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	// How familiar
	const int realFlags = Features::General::EnginePrediction::prePredictionFlags;
	const int predFlags = *localPlayer->Flags();

	const bool isOnGround = realFlags & FL_ONGROUND || realFlags & FL_PARTIALGROUND;
	const bool willBeOnGround = predFlags & FL_ONGROUND || predFlags & FL_PARTIALGROUND;

	if (isOnGround && !willBeOnGround) {
		cmd->buttons |= IN_JUMP;
	}
}

void Features::Movement::EdgeJump::SetupGUI()
{
	Features::General::EnginePrediction::ImGuiWarning();
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::EdgeJump::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)
END_SERIALIZED_STRUCT
