#include "ReturnAddr/ReturnAddr.hpp"

#include "CreateMoveHook.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/InputFlags.hpp"
#include "../../SDK/StateFlags.hpp"

#include <cstdint>
#include <cstdio>

bool Hooks::CreateMove::CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	int localPlayerIndex = Framework::ReturnAddr::invoke<int, void*>((*(void***)Interfaces::engine)[12], Interfaces::engine);
	void* localPlayer = Framework::ReturnAddr::invoke<void*, void*, int>((*(void***)Interfaces::entityList)[3], Interfaces::entityList, localPlayerIndex);

	if(!localPlayer)
		return silent;

	int flags = *(int*)(static_cast<char*>(localPlayer) + 0x13c);
	
	if(cmd->buttons & IN_JUMP) {
		if(!(flags & FL_ONGROUND)) {
			cmd->buttons &= ~IN_JUMP;
		}
	}

	return silent || true;
}
