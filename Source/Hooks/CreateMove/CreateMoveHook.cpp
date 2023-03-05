#include "CreateMoveHook.hpp"

#include <cstring>

#include "ReturnAddr/ReturnAddr.hpp"

#include "../../GameCache.hpp"
#include "../../Utils/Prediction.hpp"

#include "../../Features/General/EnginePrediction.hpp"

#include "../../Features/Movement/Bhop.hpp"
#include "../../Features/Movement/HighJump.hpp"

#include "../../Features/Legit/Triggerbot.hpp"

#include "../../Features/Semirage/Aimbot.hpp"
#include "../../Features/Semirage/RecoilAssistance.hpp"
#include "../../Features/Semirage/Backtrack.hpp"

bool __attribute((optimize("O0"))) CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	GameCache::ClearLocalPlayer();

	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(Hooks::CreateMove::hook->proxy, Memory::ret_instruction_addr, thisptr, flInputSampleTime, cmd);

	if (!cmd || !cmd->command_number)
		return silent;

	Features::Movement::Bhop::CreateMove(cmd);
	Features::Movement::HighJump::CreateMove(cmd);

	Features::General::EnginePrediction::StartPrediction(cmd);

	Features::Legit::Triggerbot::CreateMove(cmd);

	silent = !Features::Semirage::RecoilAssistance::CreateMove(cmd) && silent;
	silent = !Features::Semirage::Aimbot::CreateMove(cmd) && silent;

	// We have to keep in mind that our angles might differ from the client view at this point,
	// because of that we need to take the cmd at the last point before actually telling the server, that we shot,
	// so we have the viewangles, which is being told to the server
	Features::Semirage::Backtrack::CreateMove(cmd);

	Features::General::EnginePrediction::EndPrediction();

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
