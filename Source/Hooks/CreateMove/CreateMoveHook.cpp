#include "CreateMoveHook.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <cstring>

#include "Hooking/Hooking.hpp"
#include "ReturnAddr/ReturnAddr.hpp"

#include "../../Memory.hpp"

#include "../../Utils/VMT.hpp"

#include "../../Features/Legit/Bhop.hpp"
#include "../../Features/Legit/Triggerbot.hpp"

#include "../../Features/Semirage/Aimbot.hpp"
#include "../../Features/Semirage/RecoilAssistance.hpp"

void* createMove;

bool __attribute((optimize("O0"))) Hooks::CreateMove::CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, Memory::ret_instruction_addr, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	Features::Legit::Bhop::CreateMove(cmd);
	Features::Legit::Triggerbot::CreateMove(cmd);

	silent = silent && !Features::Semirage::RecoilAssistance::CreateMove(cmd);
	silent = silent && !Features::Semirage::Aimbot::CreateMove(cmd);

	cmd->viewangles_copy = cmd->viewangles;
	cmd->buttons_copy = cmd->buttons;

	// We can't just move the cmd into lastCmd, because it will be deleted by the original process
	if(lastCmd != nullptr)
		free(lastCmd);
	lastCmd = static_cast<CUserCmd *>(malloc(sizeof(CUserCmd)));
	memcpy((void*) lastCmd, (void*) cmd, sizeof(CUserCmd));

	return silent;
}

void Hooks::CreateMove::Hook() {	
	createMove = Utils::GetVTable(Memory::clientMode)[25];

	proxy = Framework::Hooking::detour(createMove, reinterpret_cast<void*>(CreateMoveHook), 6);
}

void Hooks::CreateMove::Unhook() {
	Framework::Memory::protect(createMove, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(createMove, proxy, 6);
	Framework::Memory::protect(createMove, PROT_READ | PROT_EXEC);
	munmap(static_cast<char*>(proxy) - FRAMEWORK_ABS_JMP_LENGTH, getpagesize());
}
