#include "RecoilAssistance.hpp"
#include "../../Interfaces.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "imgui.h"
#include "../../SDK/Definitions/InputFlags.hpp"

bool	Features::Semirage::RecoilAssistance::enabled				= false;
float	Features::Semirage::RecoilAssistance::horizontalAdjustment	= 1.0f;
float	Features::Semirage::RecoilAssistance::verticalAdjustment	= 1.0f;
bool	Features::Semirage::RecoilAssistance::silent				= false;
int		Features::Semirage::RecoilAssistance::minShots				= 1;

Vector lastAimPunch;

bool Features::Semirage::RecoilAssistance::CreateMove(CUserCmd *cmd) {
	if(!enabled || !(cmd->buttons & IN_ATTACK)) {
		lastAimPunch = Vector();
		return false;
	}

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	CBasePlayer* localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if(!localPlayer)
		return false;

	if(*localPlayer->ShotsFired() <= minShots)
		return false;

	Vector currentAimPunch = *localPlayer->AimPunchAngle();
	Vector recoil;
	if(silent)
		recoil = currentAimPunch * 2;
	else
		recoil = (currentAimPunch - lastAimPunch) * 2;
	lastAimPunch = currentAimPunch;

	recoil.x *= verticalAdjustment;
	recoil.y *= horizontalAdjustment;

	Vector correctedView = cmd->viewangles - recoil;
	correctedView.Wrap();

	cmd->viewangles = correctedView;

	if(!silent)
		Interfaces::engine->SetViewAngles(&cmd->viewangles);
	return silent;
}

void Features::Semirage::RecoilAssistance::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##SemirageRecoilAssistance"), &enabled);
	ImGui::SliderFloat(xorstr_("Horizontal adjustment##SemirageRecoilAssistance"), &horizontalAdjustment,  0.0f, 1.0f);
	ImGui::SliderFloat(xorstr_("Vertical adjustment##SemirageRecoilAssistance"), &verticalAdjustment,  0.0f, 1.0f);
	ImGui::Checkbox(xorstr_("Silent##SemirageRecoilAssistance"), &silent);
	ImGui::SliderInt(xorstr_("Min shots##SemirageRecoilAssistance"), &minShots,  0, 10);
}
