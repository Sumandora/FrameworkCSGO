#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"
#include "Memory/Memory.hpp"
#include "Assembly/Assembly.hpp"

#include "CreateMoveHook.hpp"

#include "../../Interfaces.hpp"

#include "../../Features/Legit/Bhop.hpp"

#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

void* createMove;

bool __attribute((optimize("O0"))) Hooks::CreateMove::CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, Memory::ret_instruction_addr, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	Features::Legit::Bhop::CreateMove(cmd);
	
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
