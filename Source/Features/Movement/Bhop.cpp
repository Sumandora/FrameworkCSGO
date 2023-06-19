#include "Bhop.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"

#include "../../Hooks/Game/GameFunctions.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../SDK/MoveType.hpp"

#include "../../Interfaces.hpp"
#include <cstdlib>

void Bhop::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (onlyWhenFalling && localPlayer->Velocity()->z > 0.0)
		return;

	const int flags = *localPlayer->Flags();
	const int tick = cmd->tick_count;

	static int lastJump = 0;
	static int nextRandomDelay = 0;

	if (cmd->buttons & IN_JUMP) {
		const float randomValue = (float)rand() / (float)RAND_MAX;
		if (!(flags & FL_ONGROUND)) {
			if (randomValue > inAirDistortion && tick > lastJump + nextRandomDelay)
				cmd->buttons &= ~IN_JUMP;
		} else {
			if (randomValue > hitChance || Hooks::Game::CreateMove::lastCmd.buttons & IN_JUMP /* If we held the button last tick, then we need to stop holding it now; TODO Fix this using prediction */) {
				cmd->buttons &= ~IN_JUMP;
			}
		}
	}

	if (flags & FL_ONGROUND && cmd->buttons & IN_JUMP) {
		lastJump = tick; // We just did a jump

		const float randomValue = (float)rand() / (float)RAND_MAX;
		nextRandomDelay = (int)((float)minimumReleaseDelay + (float)(maximumRelaseDelay - minimumReleaseDelay) * randomValue);
	}
}

void Bhop::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Hit chance"), &hitChance, 0, 1, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("In air distortion"), &inAirDistortion, 0, 1, xorstr_("%.2f"));
	ImGui::SliderInt(xorstr_("Minimum release delay"), &minimumReleaseDelay, 0, 10);
	ImGui::SliderInt(xorstr_("Maximum release delay"), &maximumRelaseDelay, 0, 10);
	ImGui::Checkbox(xorstr_("Only when falling"), &onlyWhenFalling);
	ImGui::HelpMarker(xorstr_("This is useful when wearing an exojump suit"));
}

SCOPED_SERIALIZER(Bhop)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE(xorstr_("Hit chance"), hitChance);
	SERIALIZE(xorstr_("In air distortion"), inAirDistortion);
	SERIALIZE(xorstr_("Minimum release delay"), minimumReleaseDelay);
	SERIALIZE(xorstr_("Maximum release delay"), maximumRelaseDelay);
	SERIALIZE(xorstr_("Only when falling"), onlyWhenFalling);
}
