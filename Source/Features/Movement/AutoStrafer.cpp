#include "AutoStrafer.hpp"

#include "../General/EnginePrediction.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"
#include "../../SDK/MoveType.hpp"

#include "../../Hooks/Game/GameFunctions.hpp"
#include "../../Interfaces.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <cstddef>
#include <optional>

static std::optional<float> lastWishDirection = 0.0f;

static void adjustButtons(CUserCmd* cmd)
{
	// Technically not needed
	if (cmd->forwardmove > 0) {
		cmd->buttons |= IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	} else if (cmd->forwardmove < 0) {
		cmd->buttons |= IN_BACK;
		cmd->buttons &= ~IN_FORWARD;
	} else {
		cmd->buttons &= ~IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	}
	if (cmd->sidemove < 0) {
		cmd->buttons |= IN_MOVELEFT;
		cmd->buttons &= ~IN_MOVERIGHT;
	} else if (cmd->sidemove > 0) {
		cmd->buttons |= IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
	} else {
		cmd->buttons &= ~IN_MOVELEFT;
		cmd->buttons &= ~IN_MOVERIGHT;
	}
}

void AutoStrafer::createMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::getLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (localPlayer->GetMoveType() == MOVETYPE_NOCLIP || localPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (*localPlayer->Flags() & FL_ONGROUND && (!enginePrediction.enabled || enginePrediction.prePredictionFlags & FL_ONGROUND)) {
		// Only abort if we are not going to be in air again (if bhopping don't abort)
		if (cmd->forwardmove == 0.0f && cmd->sidemove == 0.0f)
			lastWishDirection.reset(); // We have no direction to move to.
		else
			lastWishDirection = atan2f(-cmd->sidemove, cmd->forwardmove);
		return;
	}

	const Vector velocity = *localPlayer->Velocity();
	const float speed = velocity.HorizontalLength();

	if (speed < 5.0f)
		return;

	const float term = ConVarStorage::sv_air_max_wishspeed()->GetFloat() / ConVarStorage::sv_airaccelerate()->GetFloat() / ConVarStorage::sv_maxspeed()->GetFloat() * 100.0f / speed;

	if (term <= -1.0f || term >= 1.0f)
		return;

	const float perfectDelta = acosf(term);

	if (directional) {
		// https://github.com/degeneratehyperbola/NEPS/blob/7e41ae811cf4ae136203093a059e137c8a10796f/NEPS/Hacks/Misc.cpp#L830
		const float yaw = DEG2RAD(cmd->viewangles.y);
		const float realDirection = atan2f(velocity.y, velocity.x) - yaw;
		float wishDirection;
		if (cmd->forwardmove != 0.0f || cmd->sidemove != 0.0f)
			wishDirection = atan2f(-cmd->sidemove, cmd->forwardmove);
		else if (lastWishDirection.has_value())
			wishDirection = lastWishDirection.value(); // If we release all keys go to the last known direction
		else
			return;
		lastWishDirection = wishDirection;

		const float delta = std::remainderf(wishDirection - realDirection, 2.0f * M_PI);

		float newDirection;
		if (allowHardTurns && std::abs(delta) >= DEG2RAD(hardTurnThreshold))
			newDirection = realDirection + delta; // User wants to make a hard turn, don't smooth it, he might jump into the line of sight of an enemy
		else {
			newDirection = realDirection + (delta > 0.0 ? perfectDelta : -perfectDelta);
		}

		cmd->forwardmove = cosf(newDirection) * ConVarStorage::cl_forwardspeed()->GetFloat();
		cmd->sidemove = -sinf(newDirection) * ConVarStorage::cl_sidespeed()->GetFloat();
		adjustButtons(cmd);
	} else {
		const float oldYaw = Hooks::Game::CreateMove::lastCmd.viewangles.y;
		const float newYaw = cmd->viewangles.y;

		const float change = std::remainderf(newYaw - oldYaw, 360.0f);
		if (!onlyWhenIdle || (cmd->sidemove == 0.0f && cmd->forwardmove == 0.0f)) {
			cmd->forwardmove = 0.0f;
			if (change < 0.0)
				cmd->sidemove = ConVarStorage::cl_sidespeed()->GetFloat();
			else if (change > 0.0)
				cmd->sidemove = -ConVarStorage::cl_sidespeed()->GetFloat();
			else
				cmd->sidemove = 0;
			adjustButtons(cmd);
		}
	}
}

void AutoStrafer::setupGUI()
{
	enginePrediction.imGuiWarning();
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Checkbox("Directional", &directional);
	if (directional) {
		ImGui::Checkbox("Allow hard turns", &allowHardTurns);
		if (allowHardTurns)
			ImGui::SliderFloat("Hard turn threshold", &hardTurnThreshold, 0.0f, 180.0f, "%.2f");
	} else {
		ImGui::Checkbox("Only when idle", &onlyWhenIdle);
	}
}

SCOPED_SERIALIZER(AutoStrafer)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Directional", directional);
	SERIALIZE("Allow hard turns", allowHardTurns);
	SERIALIZE("Hard turn threshold", hardTurnThreshold);
	SERIALIZE("Only when idle", onlyWhenIdle);
}
