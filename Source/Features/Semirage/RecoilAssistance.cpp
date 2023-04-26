#include "../Semirage.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "imgui.h"

static bool enabled = false;
static float horizontalAdjustment = 1.0f;
static float verticalAdjustment = 1.0f;
static bool silent = false;
static int minShots = 1;

static Vector lastAimPunch;

bool Features::Semirage::RecoilAssistance::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !(cmd->buttons & IN_ATTACK)) {
		return false;
	}

	if (!Interfaces::engine->IsInGame())
		return false;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return false;

	const Vector currentAimPunch = *localPlayer->AimPunchAngle();

	if (*localPlayer->ShotsFired() > minShots) {
		Vector recoil;
		if (silent)
			recoil = currentAimPunch * 2.0f;
		else
			recoil = (currentAimPunch - lastAimPunch) * 2.0f;

		recoil.x *= verticalAdjustment;
		recoil.y *= horizontalAdjustment;

		Vector correctedView = cmd->viewangles - recoil;
		correctedView.Wrap();

		cmd->viewangles = correctedView;

		if (!silent)
			Interfaces::engine->SetViewAngles(&cmd->viewangles);
	}

	lastAimPunch = currentAimPunch;
	return silent;
}

void Features::Semirage::RecoilAssistance::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Horizontal adjustment"), &horizontalAdjustment, 0.0f, 1.0f);
	ImGui::SliderFloat(xorstr_("Vertical adjustment"), &verticalAdjustment, 0.0f, 1.0f);
	ImGui::Checkbox(xorstr_("Silent"), &silent);
	ImGui::SliderInt(xorstr_("Min shots"), &minShots, 0, 10);
}

BEGIN_SERIALIZED_STRUCT(Features::Semirage::RecoilAssistance::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Horizontal adjustment"), horizontalAdjustment)
SERIALIZED_TYPE(xorstr_("Vertical adjustment"), verticalAdjustment)
SERIALIZED_TYPE(xorstr_("Silent"), silent)
SERIALIZED_TYPE(xorstr_("Min shots"), minShots)
END_SERIALIZED_STRUCT
