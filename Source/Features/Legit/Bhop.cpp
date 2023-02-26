#include "Bhop.hpp"

#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"
#include "../../SDK/Definitions/StateFlags.hpp"

#include "../../SDK/GameClass/CBasePlayer.hpp"

bool	Features::Legit::Bhop::enabled	 		= false;
int		Features::Legit::Bhop::humanization		= 0;

void Features::Legit::Bhop::CreateMove(CUserCmd* cmd) {
	if(!enabled)
		return;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	CBasePlayer* localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));

	if(!localPlayer)
		return;
	
	int flags = *localPlayer->Flags();

	if(humanization > 0) {
		if(!(flags & FL_ONGROUND)) {
			if(rand() % humanization == 0)
				cmd->buttons &= ~IN_JUMP;
		} else {
			if(cmd->tick_count % (humanization + 1 /* mod 1 will always evaluate to 0 */) == 0)
				cmd->buttons &= ~IN_JUMP;
		}
	} else {
		if(cmd->buttons & IN_JUMP && !(flags & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
}

void Features::Legit::Bhop::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitBhop"), &enabled);
	ImGui::SliderInt(xorstr_("Humanize##LegitBhop"), &humanization, 0, 4);
}
