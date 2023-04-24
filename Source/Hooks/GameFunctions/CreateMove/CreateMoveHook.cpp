#include "CreateMoveHook.hpp"

#include "../../../Features/Features.hpp"

#include "../GameHook.hpp"

static GameHook* hook;

bool CreateMoveHook(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	bool silent = InvokeFunction<bool, void*, float, CUserCmd*>(hook->proxy, thisptr, flInputSampleTime, cmd);

	if (!cmd || !cmd->command_number)
		return silent;

	Features::Movement::Bhop::CreateMove(cmd);
	Features::Movement::CrouchJump::CreateMove(cmd);
	Features::Movement::FastDuck::CreateMove(cmd);

	Features::General::EnginePrediction::StartPrediction(cmd);
	{
		Features::Movement::AutoStrafer::CreateMove(cmd);
		Features::Movement::JumpBug::CreateMove(cmd);
		Features::Movement::EdgeJump::CreateMove(cmd);

		silent = !Features::Semirage::RecoilAssistance::CreateMove(cmd) && silent;
		silent = !Features::Semirage::Aimbot::CreateMove(cmd) && silent;

		Features::Legit::Triggerbot::CreateMove(cmd);

		// We have to keep in mind that our angles might differ from the client view at this point,
		// because of that we need to take the cmd at the last point before actually telling the server, that we shot,
		// so we have the viewangles, which is being told to the server
		Features::Semirage::Backtrack::CreateMove(cmd);
	}
	Features::General::EnginePrediction::EndPrediction();

	cmd->viewangles_copy = cmd->viewangles;
	cmd->buttons_copy = cmd->buttons;

	Hooks::CreateMove::lastCmd = *cmd;

	return silent;
}

void Hooks::CreateMove::Hook()
{
	hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMoveHook));
}

void Hooks::CreateMove::Unhook()
{
	delete hook;
}
