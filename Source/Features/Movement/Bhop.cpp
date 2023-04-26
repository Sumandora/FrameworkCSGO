#include "../Movement.hpp"

#include "imgui.h"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../Interfaces.hpp"
#include "../../SDK/MoveType.hpp"

static bool enabled = false;
static int humanization = 0;

void Features::Movement::Bhop::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	const int flags = *localPlayer->Flags();

	if (humanization > 0) {
		if (!(flags & FL_ONGROUND)) {
			// We don't necessarily want to tell the server, that we are able to perform tick perfect button presses, so let's randomize them in air, they matter anyway
			if (rand() % humanization == 0)
				cmd->buttons &= ~IN_JUMP;
		} else {
			if (cmd->tick_count % (humanization + 1 /* mod 1 will always evaluate to 0 */) == 0)
				cmd->buttons &= ~IN_JUMP;
		}
	} else {
		if (cmd->buttons & IN_JUMP && !(flags & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
}

void Features::Movement::Bhop::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderInt(xorstr_("Humanization"), &humanization, 0, 4);
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::Bhop::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Humanization"), humanization)
END_SERIALIZED_STRUCT
