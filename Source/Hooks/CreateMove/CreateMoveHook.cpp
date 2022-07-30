#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"
#include "Memory/Memory.hpp"

#include "CreateMoveHook.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/InputFlags.hpp"
#include "../../SDK/StateFlags.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../Utils/VMT.hpp"

#include "../../Netvars.hpp"

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
	void* hudProcessInput = Utils::GetVTable(Interfaces::baseClient)[10];
	
	/**
	 * This method calls GetClientMode()
	 * Call instruction is at the 11th byte
	 * The next byte is the address
	 */
	char* relAddr = static_cast<char*>(hudProcessInput) + 12;
	void* getClientMode = relAddr + 4 + *reinterpret_cast<int*>(relAddr);
	
	relAddr = static_cast<char*>(getClientMode) + 4; // Skip push and mov opcodes
	void* clientMode = *reinterpret_cast<void**>(relAddr + 4 + *reinterpret_cast<int*>(relAddr));
	
	createMove = Utils::GetVTable(clientMode)[25];

	proxy = Framework::Hooking::detour(createMove, reinterpret_cast<void*>(CreateMoveHook), 6);
}

void Hooks::CreateMove::Unhook() {
	Framework::Memory::protect(createMove, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(createMove, proxy, 6);
	Framework::Memory::protect(createMove, PROT_READ | PROT_EXEC);
	munmap(proxy, getpagesize());
}
