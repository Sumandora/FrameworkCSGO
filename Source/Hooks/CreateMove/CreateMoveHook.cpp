#include "CreateMoveHook.hpp"

#include <cstring>

#include "ReturnAddr/ReturnAddr.hpp"

#include "../../GameCache.hpp"
#include "../../Memory.hpp"

#include "../../Utils/VMT.hpp"

#include "../../Features/Legit/Triggerbot.hpp"
#include "../../Features/Movement/Bhop.hpp"

#include "../../Features/Semirage/Aimbot.hpp"
#include "../../Features/Semirage/RecoilAssistance.hpp"

bool __attribute((optimize("O0"))) CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	GameCache::ClearLocalPlayer();

	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(Hooks::CreateMove::hook->proxy, Memory::ret_instruction_addr, thisptr, flInputSampleTime, cmd);

	if (!cmd || !cmd->command_number)
		return silent;

	Features::Legit::Bhop::CreateMove(cmd);
	Features::Legit::Triggerbot::CreateMove(cmd);

	silent = !Features::Semirage::RecoilAssistance::CreateMove(cmd) && silent;
	silent = !Features::Semirage::Aimbot::CreateMove(cmd) && silent;

	cmd->viewangles_copy = cmd->viewangles;
	cmd->buttons_copy	 = cmd->buttons;

	// We can't just move the cmd into lastCmd, because it will be deleted by the original process
	if (Hooks::CreateMove::lastCmd != nullptr)
		free(Hooks::CreateMove::lastCmd);
	Hooks::CreateMove::lastCmd = static_cast<CUserCmd*>(malloc(sizeof(CUserCmd)));
	memcpy((void*)Hooks::CreateMove::lastCmd, (void*)cmd, sizeof(CUserCmd));

	return silent;
}

void Hooks::CreateMove::Hook() {
	hook = new class Hook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMoveHook), 6);
}

void Hooks::CreateMove::Unhook() {
	delete hook;
}
