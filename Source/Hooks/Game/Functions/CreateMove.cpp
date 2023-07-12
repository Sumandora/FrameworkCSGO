#include "../GameFunctions.hpp"

#include "../../../Features/General/EnginePrediction.hpp"
#include "../../../Features/Legit/Triggerbot.hpp"
#include "../../../Features/Movement/AutoStrafer.hpp"
#include "../../../Features/Movement/Bhop.hpp"
#include "../../../Features/Movement/CrouchJump.hpp"
#include "../../../Features/Movement/EdgeJump.hpp"
#include "../../../Features/Movement/FastDuck.hpp"
#include "../../../Features/Movement/JumpBug.hpp"
#include "../../../Features/Semirage/Aimbot.hpp"
#include "../../../Features/Semirage/Backtrack.hpp"
#include "../../../Features/Semirage/RecoilAssistance.hpp"
#include "../../../Features/Visuals/ESP/ESP.hpp"
#include "../../../Interfaces.hpp"

#include "../../../Utils/MouseCorrection.hpp"

bool Hooks::Game::CreateMove::HookFunc(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	bool silent = InvokeFunction<bool, void*, float, CUserCmd*>(hook->proxy, thisptr, flInputSampleTime, cmd);

	if (!cmd || !cmd->command_number)
		return silent;

	bhop.CreateMove(cmd);
	crouchJump.CreateMove(cmd);
	fastDuck.CreateMove(cmd);

	enginePrediction.StartPrediction(cmd);
	{
		autoStrafer.CreateMove(cmd);
		jumpBug.CreateMove(cmd);
		edgeJump.CreateMove(cmd);

		silent = !recoilAssistance.CreateMove(cmd) && silent;
		silent = !semirageAimbot.CreateMove(cmd) && silent;

		triggerbot.CreateMove(cmd);

		// We have to keep in mind that our angles might differ from the client view at this point,
		// because of that we need to take the cmd at the last point before actually telling the server, that we shot,
		// so we have the viewangles, which is being told to the server
		backtrack.CreateMove(cmd);
	}
	enginePrediction.EndPrediction();

	Utils::CorrectMouseDeltas(cmd);

	cmd->viewangles_copy = cmd->viewangles;
	cmd->buttons_copy = cmd->buttons;

	lastCmd = *cmd;

	return silent;
}
