#include "RecoilAssistance.hpp"

#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

static Vector lastAimPunch;

bool RecoilAssistance::CreateMove(CUserCmd* cmd)
{
	if (!enabled || !(cmd->buttons & IN_ATTACK)) {
		return false;
	}

	if (!Interfaces::engine->IsInGame())
		return false;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
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

void RecoilAssistance::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SliderFloat(xorstr_("Horizontal adjustment"), &horizontalAdjustment, 0.0f, 1.0f);
	ImGui::SliderFloat(xorstr_("Vertical adjustment"), &verticalAdjustment, 0.0f, 1.0f);
	ImGui::Checkbox(xorstr_("Silent"), &silent);
	ImGui::SliderInt(xorstr_("Min shots"), &minShots, 0, 10);
}

SCOPED_SERIALIZER(RecoilAssistance)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE(xorstr_("Horizontal adjustment"), horizontalAdjustment);
	SERIALIZE(xorstr_("Vertical adjustment"), verticalAdjustment);
	SERIALIZE(xorstr_("Silent"), silent);
	SERIALIZE(xorstr_("Min shots"), minShots);
}
