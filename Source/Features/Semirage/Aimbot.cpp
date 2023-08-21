#include "Aimbot.hpp"

#include "imgui.h"

#include "../General/EventLog.hpp"

#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/Keybind.hpp"
#include "../../GUI/Elements/Popup.hpp"

#include "../../Hooks/Game/GameFunctions.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../Utils/Projection.hpp"
#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include <cstring>
#include <optional>

static bool wasFaked = false;
static SemirageAimbot::WeaponConfig* lastWeaponConfig = nullptr;
static std::optional<Vector> aimTarget;

static CBasePlayer* getLocalPlayer()
{
	CBasePlayer* localPlayer = Memory::getLocalPlayer();

	if (!localPlayer || !localPlayer->IsAlive())
		return nullptr;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return nullptr;

	return localPlayer;
}

SemirageAimbot::WeaponConfig* SemirageAimbot::getWeaponConfig(CBasePlayer* localPlayer)
{
	auto* combatWeapon = static_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if (!combatWeapon)
		return nullptr;

	if (!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return nullptr;

	return weaponConfigurator.getConfig(*combatWeapon->WeaponDefinitionIndex());
}

static Vector getViewAngles(SemirageAimbot::WeaponConfig* weaponConfig, const Vector& currentViewAngles)
{
	if (weaponConfig && weaponConfig->silent && wasFaked && weaponConfig->smoothRotateToOrigin)
		return Hooks::Game::CreateMove::lastCmd.viewangles; // TODO Track this
	return currentViewAngles;
}

bool SemirageAimbot::shouldAttackPlayer(CBasePlayer* localPlayer, CBasePlayer* player)
{
	if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
		return false;

	if (!IsParticipatingTeam(*player->Team()))
		return false;

	if (!friendlyFire && !player->IsEnemy(localPlayer))
		return false;

	return true;
}

bool SemirageAimbot::canPointBeSeen(CBasePlayer* localPlayer, CBasePlayer* otherPlayer)
{
	const Vector head = otherPlayer->GetBonePosition(8);

	if (dontAimThroughSmoke && Memory::lineGoesThroughSmoke(localPlayer->GetEyePosition(), head, 1))
		return false;

	CTraceFilterEntity filter(localPlayer);
	const Trace trace = Utils::traceRay(localPlayer->GetEyePosition(), head, &filter);

	return trace.m_pEnt == otherPlayer; // The ray was able to travel to its destination
}

CBasePlayer* SemirageAimbot::findTarget(CBasePlayer* localPlayer, const Vector& viewAngles)
{
	const Vector playerEye = localPlayer->GetEyePosition();

	CBasePlayer* target = nullptr;
	float bestDistance{};

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!shouldAttackPlayer(localPlayer, player))
			continue;

		const Vector head = player->GetBonePosition(8);

		if (!canPointBeSeen(localPlayer, player))
			continue;

		const Vector viewDelta = (Utils::calculateView(playerEye, head) - viewAngles).Wrap();
		const float length = viewDelta.Length();

		if (!target || bestDistance > length) {
			target = player;
			bestDistance = length;
		}
	}

	return target;
}

bool SemirageAimbot::shouldAim(CBasePlayer* localPlayer, WeaponConfig* weaponConfig, bool attacking)
{
	if (weaponConfig->disabled)
		return false; // Those who reject aid swim alone in their struggles.

	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return false; // We are flashed - we can't aim now.

	if (weaponConfig->onlyWhenShooting) {
		if (!attacking) {
			if (autoFire) {
				return !autoFireKey.isSet() || autoFireKey.isActive();
			}
			return false;
		}
	}

	return true;
}

static void rotateToOrigin(SemirageAimbot::WeaponConfig* weaponConfig, const Vector& currentView, Vector& target)
{
	Vector delta = (target - currentView).Wrap();
	if (delta.Length() < weaponConfig->recombineViews) {
		// At this point the difference is so small, that it no longer matters
		wasFaked = false;
		return;
	}

	delta.x *= weaponConfig->verticalRotateToOriginSpeed;
	delta.y *= weaponConfig->horizontalRotateToOriginSpeed;

	target = (currentView + delta).Wrap();
	wasFaked = true;
}

static Vector setRotation(SemirageAimbot::WeaponConfig* weaponConfig, const Vector& targetView, const Vector& currentView, const Vector& aimPunch, Vector& target)
{
	Vector recoilCorrected = targetView;

	if (weaponConfig->controlRecoil) {
		Vector recoil = aimPunch * 2.0f;

		recoil.x *= weaponConfig->recoilScaleX;
		recoil.y *= weaponConfig->recoilScaleY;

		recoilCorrected -= recoil;
	}

	Vector viewDelta = (recoilCorrected - currentView).Wrap();

	if (weaponConfig->smoothOut) {
		viewDelta.x *= weaponConfig->verticalAimSpeed;
		viewDelta.y *= weaponConfig->horizontalAimSpeed;
	}

	target = currentView + viewDelta;

	target.Wrap();
	return target;
}

bool SemirageAimbot::performAutoFire(WeaponConfig* weaponConfig, bool hasTarget, CBasePlayer* localPlayer, const Vector& viewangles)
{
	if (weaponConfig->autoFireRecklessly && hasTarget)
		return true; // We got an entity in our fov, fire regardless of the trace ray...

	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return false;

	Vector forward;
	Utils::angleVectors(viewangles + *localPlayer->AimPunchAngle(), &forward);

	CTraceFilterEntity filter(localPlayer);
	const Vector playerEye = localPlayer->GetEyePosition();

	const Trace trace = Utils::traceRay(playerEye, playerEye + forward * 4096.0f, &filter);

	if (!dontAimThroughSmoke || !Memory::lineGoesThroughSmoke(playerEye, trace.endpos, 1)) {
		CBaseEntity* entity = trace.m_pEnt;
		if (entity && entity->IsPlayer() && !entity->GetDormant()) {
			auto* player = static_cast<CBasePlayer*>(entity);
			if (player->IsAlive() && !*player->GunGameImmunity()) {
				if (IsParticipatingTeam(*player->Team())) {
					if (friendlyFire || player->IsEnemy(localPlayer)) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool Relax(const Vector& currentViewAngles, Vector& targetViewAngles)
{
	if (lastWeaponConfig && lastWeaponConfig->silent && wasFaked && lastWeaponConfig->smoothRotateToOrigin) {
		rotateToOrigin(lastWeaponConfig, currentViewAngles, targetViewAngles);
		return true;
	}
	wasFaked = false;

	return false;
}

void SemirageAimbot::calculateAimTarget(CBasePlayer* localPlayer)
{
	if (!showDesyncedView || !wasFaked) {
		aimTarget.reset();
		return;
	}
	const Vector eye = localPlayer->GetEyePosition();

	const Vector fakedView = Hooks::Game::CreateMove::lastCmd.viewangles + *localPlayer->AimPunchAngle(); // The game moves your view upwards by the punch angle
	Vector forward;
	Utils::angleVectors(fakedView, &forward);

	CTraceFilterEntity filter(localPlayer);
	const Trace trace = Utils::traceRay(eye, eye + forward * 4096.0f, &filter);

	aimTarget = trace.endpos;
}

void SemirageAimbot::fireEvent(CGameEvent* gameEvent)
{
	if (!wasFaked)
		return; // No point in trying to reset our view when there is nothing to reset

	if (strcmp(gameEvent->GetName(), "player_spawn") != 0)
		return;

	if (Interfaces::engine->GetPlayerForUserID(gameEvent->GetInt("userid")) != Interfaces::engine->GetLocalPlayer())
		return;

	// We just respawned, keeping a desynced view is pointless
	wasFaked = false;
	Vector oldView = Hooks::Game::CreateMove::lastCmd.viewangles;
	Interfaces::engine->SetViewAngles(&oldView);

	eventLog.createReport("Gracefully undesynced view");
}

bool SemirageAimbot::createMove(CUserCmd* cmd)
{
	if (!enabled || !Interfaces::engine->IsInGame()) {
		wasFaked = false;
		return false;
	}

	bool willBeSilent = false;

	CBasePlayer* localPlayer = getLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive()) { // Without local player we can't shoot, can we?
		wasFaked = false;
		return false;
	}

	WeaponConfig* weaponConfig = getWeaponConfig(localPlayer);

	if (!weaponConfig) {
		if (wasFaked) {
			// We still have work to do
			rotateToOrigin(lastWeaponConfig, Hooks::Game::CreateMove::lastCmd.viewangles, cmd->viewangles);
			willBeSilent = true;
		}
		calculateAimTarget(localPlayer);
		// Mhm, no work to do. Stop here
		return willBeSilent;
	}

	const Vector viewAngles = getViewAngles(lastWeaponConfig, cmd->viewangles);
	const Vector recoilView = cmd->viewangles + *localPlayer->AimPunchAngle() * 2.0f;

	bool hasTarget = false;

	if (shouldAim(localPlayer, weaponConfig, cmd->buttons & IN_ATTACK)) {
		CBasePlayer* target = findTarget(localPlayer, recoilView);
		if (!target) {
			// We don't have a target, time to relax and try to combine the fake view and the real one again
			willBeSilent = Relax(viewAngles, cmd->viewangles);
		} else {
			// Critical moment: We are facing an enemy
			const Vector targetView = Utils::calculateView(localPlayer->GetEyePosition(), target->GetBonePosition(8));

			Vector wrappedDelta = (targetView - recoilView /*TODO Config for fake rotation fov*/).Wrap();
			wrappedDelta.x /= weaponConfig->fovScaleX;
			wrappedDelta.y /= weaponConfig->fovScaleY;
			if (wrappedDelta.Length() > weaponConfig->fov) {
				// dammit even the enemy which has the lowest view-difference is still too far away
				// Time to spent rotating back to the origin it is...
				willBeSilent = Relax(viewAngles, cmd->viewangles);
			} else {
				// No more playing around from here

				hasTarget = true;

				if (wasFaked && !weaponConfig->silent) {
					// If we were faking, then we need to calculate another view for the player, because the server view is not real
					Vector playerView{};
					Interfaces::engine->GetViewAngles(&playerView);
					setRotation(weaponConfig, targetView, playerView, *localPlayer->AimPunchAngle(), playerView);

					Interfaces::engine->SetViewAngles(&playerView);

					willBeSilent = true;
					wasFaked = true;
				}

				setRotation(weaponConfig, targetView, wasFaked ? Hooks::Game::CreateMove::lastCmd.viewangles : cmd->viewangles, *localPlayer->AimPunchAngle(), cmd->viewangles);

				if (!wasFaked) {
					lastWeaponConfig = weaponConfig; // If the user decides to switch the weapon, and he was using rotate to origin silent aim, we can't just give up...
				}

				if (weaponConfig->silent) {
					willBeSilent = true;
					wasFaked = true;
				}
			}
		}
	} else {
		// Ok, the user doesn't want our assistance at this moment.
		// Time for us to rotate back to the origin
		willBeSilent = Relax(viewAngles, cmd->viewangles);
	}

	if (autoFire) {
		if (!autoFireKey.isSet() || autoFireKey.isActive())
			if (performAutoFire(weaponConfig, hasTarget, localPlayer, cmd->viewangles)) // The user wants us to shoot for him... Let's grant his wish (only 2 remaining...)
				cmd->buttons |= IN_ATTACK; // FIRE!!!
	}

	calculateAimTarget(localPlayer);
	return willBeSilent;
}

void SemirageAimbot::imGuiRender(ImDrawList* drawList)
{
	// TODO Desync View renders when dead?
	if (!enabled || (!fovCircle && !showDesyncedView))
		return; // We should we continue, if the user doesn't want it?

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = getLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return; // Without local player we can't shoot, can we?

	if (aimTarget.has_value()) {
		ImVec2 screenspaceView;
		if (Utils::project(aimTarget.value(), screenspaceView))
			drawList->AddCircleFilled(screenspaceView, radius, viewColor); // TODO Perspective division
	}

	WeaponConfig* weaponConfig = getWeaponConfig(localPlayer);
	if (!weaponConfig)
		return;

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	const ImVec2 center(displaySize.x / 2.0f, displaySize.y / 2.0f);

	if (fovCircle) {
		const int size = 100;
		ImVec2 points[size];

		const float fov = weaponConfig->fov / Hooks::Game::OverrideView::lastViewSetup.fov * displaySize.x / 2.0f;

		for (int i = 0; i < size; i++) {
			const float rad = (float)i / (float)(size - 1) * (float)M_PI * 2.0f;
			points[i] = ImVec2(
				center.x + cosf(rad) * fov * weaponConfig->fovScaleY, // remember that x and y have to be flipped here
				center.y - sinf(rad) * fov * weaponConfig->fovScaleX);
		}

		drawList->AddPolyline(points, size, circleColor, ImDrawFlags_None, thickness);
	}
}

void SemirageAimbot::WeaponConfig::setupGUI()
{
	ImGui::Checkbox("Disabled", &disabled);
	if (disabled)
		return;

	ImGui::Checkbox("Only when shooting", &onlyWhenShooting);

	if (onlyWhenShooting && semirageAimbot.autoFire && !semirageAimbot.autoFireKey.isSet())
		ImGui::TextColored(ImGuiColors::yellow, "You are auto-firing without key, this won't make a difference");

	if (semirageAimbot.autoFire)
		ImGui::Checkbox("Auto fire recklessly", &autoFireRecklessly);

	ImGui::SliderFloat("FOV", &fov, 0.0f, 10.0f, "%.2f");
	ImGui::SameLine();
	if (ImGui::Popup("Scaling")) {
		ImGui::SliderFloat("X Scale", &fovScaleX, 0.0f, 2.0f, "%.2f");
		ImGui::SliderFloat("Y Scale", &fovScaleY, 0.0f, 2.0f, "%.2f");
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Control recoil", &controlRecoil);
	ImGui::SameLine();
	if (ImGui::Popup("Recoil")) {
		ImGui::SliderFloat("X Scale", &recoilScaleX, 0.0f, 1.5f, "%.2f");
		ImGui::SliderFloat("Y Scale", &recoilScaleY, 0.0f, 1.5f, "%.2f");
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Smooth out", &smoothOut);
	ImGui::SameLine();
	if (ImGui::Popup("Aim speed")) {
		ImGui::SliderFloat("Horizontal aim speed", &horizontalAimSpeed, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("Vertical aim speed", &verticalAimSpeed, 0.0f, 1.0f, "%.2f");
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Silent", &silent);

	ImGui::SameLine();
	if (ImGui::Popup("Silent")) {
		ImGui::Checkbox("Smooth rotate to origin", &smoothRotateToOrigin);
		if (smoothRotateToOrigin) {
			ImGui::SliderFloat("Horizontal speed", &horizontalRotateToOriginSpeed, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat("Vertical speed", &verticalRotateToOriginSpeed, 0.0f, 1.0f, "%.2f");

			ImGui::SliderFloat("Recombine views", &recombineViews, 0.0f, 1.0f, "%.2f");
		}
		ImGui::EndPopup();
	}
}

void SemirageAimbot::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);

	ImGui::Checkbox("Auto fire", &autoFire);
	if (autoFire)
		ImGui::InputSelector("Auto fire key (%s)", autoFireKey);

	ImGui::SliderInt("Maximal flash amount", &maximalFlashAmount, 0, 255);
	ImGui::Checkbox("Don't aim through smoke", &dontAimThroughSmoke);
	ImGui::Checkbox("Friendly fire", &friendlyFire);

	ImGui::Checkbox("FOV Circle", &fovCircle);
	ImGui::SameLine();
	if (ImGui::Popup("FOV Circle settings")) {
		ImGui::ClickableColorButton("Color", circleColor);
		ImGui::SliderFloat("Thickness", &thickness, 0.0f, 10.0f, "%.2f");
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Show desynced view", &showDesyncedView);
	ImGui::SameLine();
	if (ImGui::Popup("Show desynced view settings")) {
		ImGui::ClickableColorButton("Color", viewColor);
		ImGui::SliderFloat("Radius", &radius, 0.0f, 10.0f, "%.2f");
		ImGui::EndPopup();
	}

	weaponConfigurator.setupGUI();
}

SCOPED_SERIALIZER(SemirageAimbot::WeaponConfig)
{
	SERIALIZE("Disabled", disabled);

	SERIALIZE("Only when shooting", onlyWhenShooting);

	SERIALIZE("Auto fire recklessly", autoFireRecklessly);

	SERIALIZE("FOV", fov);
	SERIALIZE("FOV scale X", fovScaleX);
	SERIALIZE("FOV scale Y", fovScaleY);

	SERIALIZE("Control recoil", controlRecoil);
	SERIALIZE("Recoil scale X", recoilScaleX);
	SERIALIZE("Recoil scale Y", recoilScaleY);

	SERIALIZE("Smooth out", smoothOut);
	SERIALIZE("Horizontal aim speed", horizontalAimSpeed);
	SERIALIZE("Vertical aim speed", verticalAimSpeed);

	SERIALIZE("Silent", silent);

	SERIALIZE("Smooth rotate to origin", smoothRotateToOrigin);
	SERIALIZE("Horizontal rotate to origin speed", horizontalRotateToOriginSpeed);
	SERIALIZE("Vertical rotate to origin speed", verticalRotateToOriginSpeed);
	SERIALIZE("Recombine views", recombineViews);
}

SCOPED_SERIALIZER(SemirageAimbot)
{
	SERIALIZE("Enabled", enabled);

	SERIALIZE("Auto fire", autoFire);
	SERIALIZE_STRUCT("Auto fire key", autoFireKey);

	SERIALIZE("Maximal flash amount", maximalFlashAmount);
	SERIALIZE("Don't aim through smoke", dontAimThroughSmoke);
	SERIALIZE("Friendly fire", friendlyFire);

	SERIALIZE_STRUCT("Weapons", weaponConfigurator);

	SERIALIZE("FOV Circle", fovCircle);
	SERIALIZE_VECTOR4D("Circle color", circleColor.Value);
	SERIALIZE("Thickness", thickness);

	SERIALIZE("Show desynced view", showDesyncedView);
	SERIALIZE_VECTOR4D("View color", viewColor.Value);
	SERIALIZE("Radius", radius);
}