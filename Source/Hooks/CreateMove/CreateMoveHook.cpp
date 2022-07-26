#include "ReturnAddr/ReturnAddr.hpp"
#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"

#include "CreateMoveHook.hpp"
#include "../../Interfaces.hpp"

#include "../../SDK/InputFlags.hpp"
#include "../../SDK/StateFlags.hpp"
#include "../../Utils/VMT.hpp"

#include <cstdint>
#include <cstdio>

bool Hooks::CreateMove::CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	// Set the address for the return address spoofer
	if(!Framework::ReturnAddr::ret_instruction_addr)
		Framework::ReturnAddr::ret_instruction_addr =
			Framework::ReturnAddr::leave_ret_instruction.searchPattern(
				__builtin_extract_return_addr(__builtin_return_address(0))
			);

	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	void* localPlayer = Interfaces::entityList->GetClientEntity(localPlayerIndex);

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
	void* clientMode = *(void**) (relAddr + 4 + *reinterpret_cast<int*>(relAddr));
	
	void* createMove = Utils::GetVTable(clientMode)[25];

	proxy = Framework::Hooking::detour(createMove, (void*) CreateMoveHook, 6);
}
