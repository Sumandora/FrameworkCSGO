#include "EdgeJump.hpp"

#include "../General/EnginePrediction.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../Interfaces.hpp"

void EdgeJump::CreateMove(CUserCmd* cmd)
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

	// How familiar
	const int realFlags = enginePrediction.prePredictionFlags;
	const int predFlags = *localPlayer->Flags();

	const bool isOnGround = realFlags & FL_ONGROUND || realFlags & FL_PARTIALGROUND;
	const bool willBeOnGround = predFlags & FL_ONGROUND || predFlags & FL_PARTIALGROUND;

	if (isOnGround && !willBeOnGround) {
		cmd->buttons |= IN_JUMP;
	}
}

void EdgeJump::SetupGUI()
{
	enginePrediction.ImGuiWarning();
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::InputSelector("Input (%s)", input);
}

SCOPED_SERIALIZER(EdgeJump)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_STRUCT("Input", input);
}
