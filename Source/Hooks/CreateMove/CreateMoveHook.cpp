#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"
#include "Memory/Memory.hpp"
#include "Assembly/Assembly.hpp"

#include "CreateMoveHook.hpp"

#include "../../Interfaces.hpp"

#include "../../SDK/InputFlags.hpp"
#include "../../SDK/StateFlags.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../Utils/VMT.hpp"

#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

void* createMove;

bool __attribute((optimize("O0"))) Hooks::CreateMove::CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, Memory::ret_instruction_addr, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	C_BasePlayer* localPlayer = reinterpret_cast<C_BasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));

	if(!localPlayer)
		return silent;
	
	int flags = *localPlayer->Flags();
	
	if(cmd->buttons & IN_JUMP) {
		if(!(flags & FL_ONGROUND)) {
			cmd->buttons &= ~IN_JUMP;
		}
	}
	
	return silent || true;
}

void Hooks::CreateMove::Hook() {	
	createMove = Utils::GetVTable(Interfaces::clientMode)[25];

	proxy = Framework::Hooking::detour(createMove, reinterpret_cast<void*>(CreateMoveHook), 6);
}

void Hooks::CreateMove::Unhook() {
	Framework::Memory::protect(createMove, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(createMove, proxy, 6);
	Framework::Memory::protect(createMove, PROT_READ | PROT_EXEC);
	munmap(static_cast<char*>(proxy) - FRAMEWORK_ABS_JMP_LENGTH, getpagesize());
}
