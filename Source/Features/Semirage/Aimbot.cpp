#include "../Semirage.hpp"

#include "imgui.h"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/Popup.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../Hooks/GameFunctions/CreateMove/CreateMoveHook.hpp"
#include "../../Hooks/GameFunctions/FrameStageNotify/FrameStageNotifyHook.hpp"
#include "../../Hooks/GameFunctions/OverrideView/OverrideViewHook.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include "../Visuals.hpp"

#include <algorithm>
static bool enabled = false;

struct SemirageAimbotWeaponConfig {
	bool onlyWhenShooting = false; // TODO Separate key

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

bool wasFaked = false;

// TODO Rename/Rewrite
SemirageAimbotWeaponConfig* GetWeaponConfig()
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return nullptr;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return nullptr;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return nullptr;

	auto* combatWeapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if (!combatWeapon)
		return nullptr;

	if (!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return nullptr;

	return weaponConfigurator.getConfig(*combatWeapon->WeaponDefinitionIndex());
}

bool Features::Semirage::Aimbot::CreateMove(CUserCmd* cmd)
{
	SemirageAimbotWeaponConfig* weaponConfig = GetWeaponConfig();
	if (!weaponConfig)
		return false;
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	Vector viewAngles;
	if (weaponConfig->silent && wasFaked && weaponConfig->smoothRotateToOrigin)
		viewAngles = Hooks::CreateMove::lastCmd.viewangles;
	else
		viewAngles = Vector(cmd->viewangles);

	CBasePlayer* target = nullptr;
	Vector bestRotation;
	float bestDistance {};

	// For compatibility’s sake, play it off like we didn't find a target
	if ((cmd->buttons & IN_ATTACK || !weaponConfig->onlyWhenShooting) && localPlayer->GetFlashAlpha() <= (float)maximalFlashAmount) {
		CTraceFilterEntity filter(localPlayer);

		// The first object is always the WorldObj
		for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
			auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
			if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
				continue;

			if (!IsParticipatingTeam(*player->Team()))
				continue;

			if (!(friendlyFire || player->IsEnemy(localPlayer)))
				continue;

			const Vector playerEye = localPlayer->GetEyePosition();

			Matrix3x4* boneMatrix = player->SetupBones();

			const Vector head = boneMatrix[8].Origin();

			if (dontAimThroughSmoke && Memory::LineGoesThroughSmoke(playerEye, head, 1))
				continue;

			const Trace trace = Utils::TraceRay(playerEye, head, &filter);

			if (trace.m_pEnt != player)
				continue; // The enemy is behind something...

			Vector rotation = Utils::CalculateView(playerEye, head);
			if (weaponConfig->controlRecoil) {
				Vector recoil = *localPlayer->AimPunchAngle() * ConVarStorage::weapon_recoil_scale()->GetFloat();
				recoil.x *= weaponConfig->recoilScaleX;
				recoil.y *= weaponConfig->recoilScaleY;

				rotation -= recoil;
			}
			Vector wrappedDelta = (rotation - cmd->viewangles).Wrap();
			wrappedDelta.x /= weaponConfig->fovScaleX;
			wrappedDelta.y /= weaponConfig->fovScaleY;
			const float length = wrappedDelta.Length(); // Use the real angle for the fov check
			rotation -= viewAngles;
			rotation.Wrap();

			if (!target || bestDistance > length) {
				target = player;
				bestDistance = length;
				bestRotation = rotation;
			}
		}
	}

	if (!target || bestDistance > weaponConfig->fov) {
		if (weaponConfig->silent && wasFaked && weaponConfig->smoothRotateToOrigin) {
			Vector delta = cmd->viewangles - viewAngles;
			delta.Wrap();
			if (delta.Length() < weaponConfig->recombineViews) {
				// At this point the difference is so small, that it no longer matters
				wasFaked = false;
				return false;
			}

			delta.x *= weaponConfig->verticalRotateToOriginSpeed;
			delta.y *= weaponConfig->horizontalRotateToOriginSpeed;

			cmd->viewangles = viewAngles + delta;
			cmd->viewangles.Wrap();
			wasFaked = true;
			return true;
		}
		wasFaked = false;
		return false;
	}

	if (weaponConfig->smoothOut) {
		bestRotation.x *= weaponConfig->verticalAimSpeed;
		bestRotation.y *= weaponConfig->horizontalAimSpeed;
	}

	cmd->viewangles = viewAngles + bestRotation;
	cmd->viewangles.Wrap();
	if (!weaponConfig->silent)
		Interfaces::engine->SetViewAngles(&cmd->viewangles);

	wasFaked = true;
	return weaponConfig->silent;
}

void Features::Semirage::Aimbot::ImGuiRender(ImDrawList* drawList)
{
	if (!fovCircle && !showDesyncedView)
		return;

	SemirageAimbotWeaponConfig* weaponConfig = GetWeaponConfig();
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

	if (showDesyncedView && wasFaked) {
		CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
		Vector eye = localPlayer->GetEyePosition();

		Vector fakedView = Hooks::CreateMove::lastCmd.viewangles;
		Vector forward;
		Utils::AngleVectors(fakedView, &forward);

		CTraceFilterEntity filter(localPlayer);
		Trace trace = Utils::TraceRay(eye, eye + forward * 4096.0f, &filter); // TODO TraceRay in CreateMove

		ImVec2 screenspaceView;
		if (Features::Visuals::Esp::WorldToScreen(Hooks::FrameStageNotify::worldToScreenMatrix, trace.endpos, screenspaceView))
			drawList->AddCircleFilled(screenspaceView, radius, viewColor);
	}
}

void WeaponGUI(SemirageAimbotWeaponConfig& weaponConfig)
{
	ImGui::Checkbox(xorstr_("Only when shooting"), &weaponConfig.onlyWhenShooting);

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
