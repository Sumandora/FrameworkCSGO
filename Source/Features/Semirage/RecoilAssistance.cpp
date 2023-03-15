#include "../Semirage.hpp"

#include "../../ConVarStorage.hpp"
#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

#include "imgui.h"

bool Features::Semirage::RecoilAssistance::enabled = false;
float Features::Semirage::RecoilAssistance::horizontalAdjustment = 1.0F;
float Features::Semirage::RecoilAssistance::verticalAdjustment = 1.0F;
bool Features::Semirage::RecoilAssistance::silent = false;
int Features::Semirage::RecoilAssistance::minShots = 1;

Vector lastAimPunch;

bool Features::Semirage::RecoilAssistance::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !(cmd->buttons & IN_ATTACK)) {
		return false;
	}

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return false;

	const Vector currentAimPunch = *localPlayer->AimPunchAngle();

	if (*localPlayer->ShotsFired() > minShots) {
		Vector recoil;
		if (silent)
			recoil = currentAimPunch * ConVarStorage::weapon_recoil_scale->GetFloat();
		else
			recoil = (currentAimPunch - lastAimPunch) * ConVarStorage::weapon_recoil_scale->GetFloat();

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

BEGIN_SERIALIZED_STRUCT(Features::Semirage::RecoilAssistance::Serializer, xorstr_("Recoil assistance"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Horizontal adjustment"), horizontalAdjustment)
SERIALIZED_TYPE(xorstr_("Vertical adjustment"), verticalAdjustment)
SERIALIZED_TYPE(xorstr_("Silent"), silent)
SERIALIZED_TYPE(xorstr_("Min shots"), minShots)
END_SERIALIZED_STRUCT
