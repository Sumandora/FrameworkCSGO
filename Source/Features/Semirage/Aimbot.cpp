#include "../Semirage.hpp"

#include "imgui.h"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/Popup.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../Hooks/CreateMove/CreateMoveHook.hpp"
#include "../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"
#include "../../Hooks/OverrideView/OverrideViewHook.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../Visuals.hpp"

#include <algorithm>

static bool enabled = false;
static int input = ImGuiKey_None;
static bool autoFire = false;
static bool autoFireOnKey = true;
// TODO Auto Fire regardless of ray intersection

struct SemirageAimbotWeaponConfig {
	bool onlyWhenShooting = false;
	bool orWhenHoldingInput = true;

	float fov = 3.0f;
	float fovScaleX = 1.0f;
	float fovScaleY = 1.0f;

	bool controlRecoil = false;
	float recoilScaleX = 1.0f;
	float recoilScaleY = 1.0f;

	bool smoothOut = false;
	float horizontalAimSpeed = 0.2f;
	float verticalAimSpeed = 0.2f;

	bool silent = false;

	bool smoothRotateToOrigin = false;
	float horizontalRotateToOriginSpeed = 0.2f;
	float verticalRotateToOriginSpeed = 0.2f;
	float recombineViews = 0.1f;

	DECLARE_SERIALIZER(Serializer);
};
void WeaponGUI(SemirageAimbotWeaponConfig& weaponConfig);

static WeaponConfigurator<SemirageAimbotWeaponConfig> weaponConfigurator(WeaponGUI);

static int maximalFlashAmount = 255;
static bool dontAimThroughSmoke = false;
static bool friendlyFire = false;

static bool fovCircle = false; // Technically an ellipse if you use FOV scaling
static ImColor circleColor = ImGuiColors::white;
static float thickness = 1.0f;

static bool showDesyncedView = false;
static ImColor viewColor = ImGuiColors::red;
static float radius = 5.0f;

static bool wasFaked = false;
static SemirageAimbotWeaponConfig* lastWeaponConfig = nullptr;

CBasePlayer* GetLocalPlayer()
{
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (!localPlayer || !localPlayer->IsAlive())
		return nullptr;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return nullptr;

	return localPlayer;
}

SemirageAimbotWeaponConfig* GetWeaponConfig(CBasePlayer* localPlayer)
{
	auto* combatWeapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if (!combatWeapon)
		return nullptr;

	if (!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return nullptr;

	return weaponConfigurator.GetConfig(*combatWeapon->WeaponDefinitionIndex());
}

Vector GetViewAngles(SemirageAimbotWeaponConfig* weaponConfig, const Vector& currentViewAngles)
{
	if (weaponConfig && weaponConfig->silent && wasFaked && weaponConfig->smoothRotateToOrigin)
		return Hooks::CreateMove::lastCmd.viewangles; // TODO Track this
	else
		return currentViewAngles;
}

bool ShouldAttackPlayer(CBasePlayer* localPlayer, CBasePlayer* player)
{
	if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
		return false;

	if (!IsParticipatingTeam(*player->Team()))
		return false;

	if (!friendlyFire && !player->IsEnemy(localPlayer))
		return false;

	return true;
}

bool CanPointBeSeen(CBasePlayer* localPlayer, const Vector& point)
{
	if (dontAimThroughSmoke && Memory::LineGoesThroughSmoke(localPlayer->GetEyePosition(), point, 1))
		return false;

	CTraceFilterNoEntities filter;
	const Trace trace = Utils::TraceRay(localPlayer->GetEyePosition(), point, &filter);

	return trace.fraction == 1.0f; // The ray was able to travel to its destination
}

CBasePlayer* FindTarget(CBasePlayer* localPlayer, const Vector& viewAngles)
{
	const Vector playerEye = localPlayer->GetEyePosition();

	CBasePlayer* target = nullptr;
	float bestDistance {};

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!ShouldAttackPlayer(localPlayer, player))
			continue;

		Vector head = player->GetBonePosition(8);

		if (!CanPointBeSeen(localPlayer, head))
			continue;

		Vector viewDelta = (Utils::CalculateView(playerEye, head) - viewAngles).Wrap();
		float length = viewDelta.Length();

		if (!target || bestDistance > length) {
			target = player;
			bestDistance = length;
		}
	}

	return target;
}

bool ShouldAim(CBasePlayer* localPlayer, SemirageAimbotWeaponConfig* weaponConfig, bool attacking)
{
	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return false; // We are flashed - we can't aim now.

	if (weaponConfig->onlyWhenShooting) {
		if (!attacking) {
			if (weaponConfig->orWhenHoldingInput) {
				if (!IsInputDown(input, true))
					return false;
			}
			return false;
		}
	}

	return true;
}

void RotateToOrigin(SemirageAimbotWeaponConfig* weaponConfig, const Vector& currentView, Vector& target)
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

Vector SetRotation(SemirageAimbotWeaponConfig* weaponConfig, const Vector& targetView, const Vector& currentView, const Vector& aimPunch, Vector& target)
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
}

bool AutoFire(CBasePlayer* localPlayer, const Vector& viewangles)
{
	Vector forward;
	Utils::AngleVectors(viewangles + *localPlayer->AimPunchAngle(), &forward);

	CTraceFilterEntity filter(localPlayer);
	Vector playerEye = localPlayer->GetEyePosition();

	Trace trace = Utils::TraceRay(playerEye, playerEye + forward * 4096.0f, &filter);

	if (!dontAimThroughSmoke || !Memory::LineGoesThroughSmoke(playerEye, trace.endpos, 1)) {
		CBaseEntity* entity = trace.m_pEnt;
		if (entity && entity->IsPlayer() && !entity->GetDormant()) {
			auto* player = reinterpret_cast<CBasePlayer*>(entity);
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

bool Relax(const Vector& currentViewAngles, Vector& targetViewAngles) {
	if (lastWeaponConfig && lastWeaponConfig->silent && wasFaked && lastWeaponConfig->smoothRotateToOrigin) {
		RotateToOrigin(lastWeaponConfig, currentViewAngles, targetViewAngles);
		return true;
	} else {
		wasFaked = false;
	}
	return false;
}

bool Features::Semirage::Aimbot::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return false;

	bool willBeSilent = false;

	CBasePlayer* localPlayer = GetLocalPlayer();
	if (!localPlayer)
		return false; // Without local player we can't shoot, can we?

	SemirageAimbotWeaponConfig* weaponConfig = GetWeaponConfig(localPlayer);

	/*if(lastWeaponConfig && lastWeaponConfig->silent && lastWeaponConfig->smoothRotateToOrigin && (!weaponConfig || !weaponConfig->silent || !weaponConfig->smoothRotateToOrigin)) {
		if(wasFaked) {
			// This is a critical moment: The user just switched their gun, but we still have work to do (smooth out the snap)
			RotateToOrigin(lastWeaponConfig, Hooks::CreateMove::lastCmd.viewangles, cmd->viewangles);
			willBeSilent = true;
		}
	}*/

	if (!weaponConfig) {
		if (wasFaked) {
			// We still have work to do
			RotateToOrigin(lastWeaponConfig, Hooks::CreateMove::lastCmd.viewangles, cmd->viewangles);
			willBeSilent = true;
		}
		// Mhm, no work to do. Stop here
		return willBeSilent;
	}

	Vector viewAngles = GetViewAngles(lastWeaponConfig, cmd->viewangles);
	Vector recoilView = cmd->viewangles + *localPlayer->AimPunchAngle() * 2.0f;

	if (ShouldAim(localPlayer, weaponConfig, cmd->buttons & IN_ATTACK)) {
		CBasePlayer* target = FindTarget(localPlayer, recoilView);
		if (!target) {
			// We don't have a target, time to relax and try to combine the fake view and the real one again
			willBeSilent = Relax(viewAngles, cmd->viewangles);
		} else {
			// Critical moment: We are facing an enemy
			Vector targetView = Utils::CalculateView(localPlayer->GetEyePosition(), target->GetBonePosition(8));

			Vector wrappedDelta = (targetView - recoilView /*TODO Config for fake rotation fov*/).Wrap();
			wrappedDelta.x /= weaponConfig->fovScaleX;
			wrappedDelta.y /= weaponConfig->fovScaleY;
			if (wrappedDelta.Length() > weaponConfig->fov) {
				// dammit even the enemy which has the lowest view-difference is still too far away
				// Time to spent rotating back to the origin it is...
				willBeSilent = Relax(viewAngles, cmd->viewangles);
			} else {
				// No more playing around from here

				if (wasFaked && !weaponConfig->silent) {
					// If we were faking, then we need to calculate another view for the player, because the server view is not real
					Vector playerView{};
					Interfaces::engine->GetViewAngles(&playerView);
					SetRotation(weaponConfig, targetView, playerView, *localPlayer->AimPunchAngle(), playerView);

					Interfaces::engine->SetViewAngles(&playerView);

					willBeSilent = true;
					wasFaked = true;
				}

				SetRotation(weaponConfig, targetView, wasFaked ? Hooks::CreateMove::lastCmd.viewangles : cmd->viewangles, *localPlayer->AimPunchAngle(), cmd->viewangles);

				if(!wasFaked) {
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
		if (!autoFireOnKey || IsInputDown(input, true))
			if (AutoFire(localPlayer, cmd->viewangles)) // The user wants us to shoot for him... Let's grant his wish (only 2 remaining...)
				cmd->buttons |= IN_ATTACK; // FIRE!!!
	}

	return willBeSilent;
}

void Features::Semirage::Aimbot::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || (!fovCircle && !showDesyncedView))
		return; // We should we continue, if the user doesn't want it?

	CBasePlayer* localPlayer = GetLocalPlayer();
	if (!localPlayer)
		return; // Without local player we can't shoot, can we?

	if (showDesyncedView && wasFaked) {
		Vector eye = localPlayer->GetEyePosition();

		Vector fakedView = Hooks::CreateMove::lastCmd.viewangles + *localPlayer->AimPunchAngle(); // The game moves your view upwards by the punch angle
		Vector forward;
		Utils::AngleVectors(fakedView, &forward);

		CTraceFilterEntity filter(localPlayer);
		Trace trace = Utils::TraceRay(eye, eye + forward * 4096.0f, &filter);

		ImVec2 screenspaceView;
		if (Features::Visuals::Esp::WorldToScreen(Hooks::FrameStageNotify::worldToScreenMatrix, trace.endpos, screenspaceView))
			drawList->AddCircleFilled(screenspaceView, radius, viewColor); // TODO Perspective division
	}

	SemirageAimbotWeaponConfig* weaponConfig = GetWeaponConfig(localPlayer);
	if (!weaponConfig)
		return;

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 center(displaySize.x / 2.0f, displaySize.y / 2.0f);

	if (fovCircle) {
		int size = 100;
		ImVec2 points[size];

		float fov = weaponConfig->fov / Hooks::OverrideView::lastViewSetup.fov * displaySize.x / 2.0f;

		for (int i = 0; i < size; i++) {
			float rad = (float)i / (float)(size - 1) * (float)M_PI * 2.0f;
			points[i] = ImVec2(
				center.x + cosf(rad) * fov * weaponConfig->fovScaleY, // remember that x and y have to be flipped here
				center.y - sinf(rad) * fov * weaponConfig->fovScaleX);
		}

		drawList->AddPolyline(points, size, circleColor, ImDrawFlags_None, thickness);
	}
}

void WeaponGUI(SemirageAimbotWeaponConfig& weaponConfig)
{
	ImGui::Checkbox(xorstr_("Only when shooting"), &weaponConfig.onlyWhenShooting);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Input"))) {
		ImGui::Checkbox(xorstr_("Or when holding input"), &weaponConfig.orWhenHoldingInput);
		ImGui::EndPopup();
	}

	ImGui::SliderFloat(xorstr_("FOV"), &weaponConfig.fov, 0.0f, 10.0f, xorstr_("%.2f"));
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Scaling"))) {
		ImGui::SliderFloat(xorstr_("X Scale"), &weaponConfig.fovScaleX, 0.0f, 2.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Y Scale"), &weaponConfig.fovScaleY, 0.0f, 2.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	ImGui::Checkbox(xorstr_("Control recoil"), &weaponConfig.controlRecoil);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Recoil"))) {
		ImGui::SliderFloat(xorstr_("X Scale"), &weaponConfig.recoilScaleX, 0.0f, 1.5f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Y Scale"), &weaponConfig.recoilScaleY, 0.0f, 1.5f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	ImGui::Checkbox(xorstr_("Smooth out"), &weaponConfig.smoothOut);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Aim speed"))) {
		ImGui::SliderFloat(xorstr_("Horizontal aim speed"), &weaponConfig.horizontalAimSpeed, 0.0f, 1.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Vertical aim speed"), &weaponConfig.verticalAimSpeed, 0.0f, 1.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	ImGui::Checkbox(xorstr_("Silent"), &weaponConfig.silent);

	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Silent"))) {
		ImGui::Checkbox(xorstr_("Smooth rotate to origin"), &weaponConfig.smoothRotateToOrigin);
		if (weaponConfig.smoothRotateToOrigin) {
			ImGui::SliderFloat(xorstr_("Horizontal speed"), &weaponConfig.horizontalRotateToOriginSpeed, 0.0f, 1.0f, xorstr_("%.2f"));
			ImGui::SliderFloat(xorstr_("Vertical speed"), &weaponConfig.verticalRotateToOriginSpeed, 0.0f, 1.0f, xorstr_("%.2f"));

			ImGui::SliderFloat(xorstr_("Recombine views"), &weaponConfig.recombineViews, 0.0f, 1.0f, xorstr_("%.2f"));
		}
		ImGui::EndPopup();
	}
}

void Features::Semirage::Aimbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::InputSelector(xorstr_("Input (%s)"), input);

	ImGui::Checkbox(xorstr_("Auto fire"), &autoFire);
	if (autoFire)
		ImGui::Checkbox(xorstr_("Auto fire on key"), &autoFireOnKey);

	ImGui::SliderInt(xorstr_("Maximal flash amount"), &maximalFlashAmount, 0, 255);
	ImGui::Checkbox(xorstr_("Don't aim through smoke"), &dontAimThroughSmoke);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);

	ImGui::Checkbox(xorstr_("FOV Circle"), &fovCircle);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("FOV Circle settings"))) {
		ImGui::ClickableColorButton(xorstr_("Color"), circleColor);
		ImGui::SliderFloat(xorstr_("Thickness"), &thickness, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	ImGui::Checkbox(xorstr_("Show desynced view"), &showDesyncedView);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Show desynced view settings"))) {
		ImGui::ClickableColorButton(xorstr_("Color"), viewColor);
		ImGui::SliderFloat(xorstr_("Radius"), &radius, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	weaponConfigurator.SetupGUI();
}

BEGIN_SERIALIZED_STRUCT(SemirageAimbotWeaponConfig::Serializer)
SERIALIZED_TYPE(xorstr_("Only when shooting"), onlyWhenShooting)
SERIALIZED_TYPE(xorstr_("Or when holding input"), orWhenHoldingInput)

SERIALIZED_TYPE(xorstr_("FOV"), fov)
SERIALIZED_TYPE(xorstr_("FOV scale X"), fovScaleX)
SERIALIZED_TYPE(xorstr_("FOV scale Y"), fovScaleY)

SERIALIZED_TYPE(xorstr_("Control recoil"), controlRecoil)
SERIALIZED_TYPE(xorstr_("Recoil scale X"), recoilScaleX)
SERIALIZED_TYPE(xorstr_("Recoil scale Y"), recoilScaleY)

SERIALIZED_TYPE(xorstr_("Smooth out"), smoothOut)
SERIALIZED_TYPE(xorstr_("Horizontal aim speed"), horizontalAimSpeed)
SERIALIZED_TYPE(xorstr_("Vertical aim speed"), verticalAimSpeed)

SERIALIZED_TYPE(xorstr_("Silent"), silent)

SERIALIZED_TYPE(xorstr_("Smooth rotate to origin"), smoothRotateToOrigin)
SERIALIZED_TYPE(xorstr_("Horizontal rotate to origin speed"), horizontalRotateToOriginSpeed)
SERIALIZED_TYPE(xorstr_("Vertical rotate to origin speed"), verticalRotateToOriginSpeed)
SERIALIZED_TYPE(xorstr_("Recombine views"), recombineViews)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Semirage::Aimbot::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Input"), input)

SERIALIZED_TYPE(xorstr_("Auto fire"), autoFire)
SERIALIZED_TYPE(xorstr_("Auto fire on key"), autoFireOnKey)

SERIALIZED_TYPE(xorstr_("Maximal flash amount"), maximalFlashAmount)
SERIALIZED_TYPE(xorstr_("Don't aim through smoke"), dontAimThroughSmoke)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)

SERIALIZED_STRUCTURE(xorstr_("Weapons"), weaponConfigurator)

SERIALIZED_TYPE(xorstr_("FOV Circle"), fovCircle)
SERIALIZED_TYPE(xorstr_("Circle color"), circleColor)
SERIALIZED_TYPE(xorstr_("Thickness"), thickness)

SERIALIZED_TYPE(xorstr_("Show desynced view"), showDesyncedView)
SERIALIZED_TYPE(xorstr_("View color"), viewColor)
SERIALIZED_TYPE(xorstr_("Radius"), radius)
END_SERIALIZED_STRUCT