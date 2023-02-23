#include "Bhop.hpp"

#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"

// Settings
bool	Features::Legit::Bhop::enabled	= false;

void Features::Legit::Bhop::CreateMove(CUserCmd* cmd) {
	if(!enabled)
		return;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	CBasePlayer* localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));

	if(!localPlayer)
		return;
	
	int flags = *localPlayer->Flags();
	
	if(cmd->buttons & IN_JUMP) {
		if(!(flags & FL_ONGROUND)) {
			cmd->buttons &= ~IN_JUMP;
		}
	}
}

void Features::Legit::Bhop::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##Bhop"), &enabled);
}
