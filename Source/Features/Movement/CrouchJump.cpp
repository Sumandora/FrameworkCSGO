#include "CrouchJump.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../Interfaces.hpp"
#include "../../SDK/MoveType.hpp"

static bool performing = false;

void CrouchJump::CreateMove(CUserCmd* cmd)
{
	if (!enabled || (input.IsSet() && !input.IsActive())) {
		performing = false;
		return;
	}

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
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

void CrouchJump::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::InputSelector("Input (%s)", input);
}

SCOPED_SERIALIZER(CrouchJump)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_STRUCT("Input", input);
}
