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

bool Hooks::Game::CreateMove::hookFunc(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	bool silent = invokeFunction<bool, void*, float, CUserCmd*>(hook->proxy, thisptr, flInputSampleTime, cmd);

	if (!cmd || !cmd->command_number)
		return silent;

	bhop.createMove(cmd);
	crouchJump.createMove(cmd);
	fastDuck.createMove(cmd);

	enginePrediction.startPrediction(cmd);
	{
		autoStrafer.createMove(cmd);
		jumpBug.createMove(cmd);
		edgeJump.createMove(cmd);

		silent = !recoilAssistance.createMove(cmd) && silent;
		silent = !semirageAimbot.createMove(cmd) && silent;

		triggerbot.createMove(cmd);

		// We have to keep in mind that our angles might differ from the client view at this point,
		// because of that we need to take the cmd at the last point before actually telling the server, that we shot,
		// so we have the viewangles, which is being told to the server
		backtrack.createMove(cmd);
	}
	enginePrediction.endPrediction();

	Utils::correctMouseDeltas(cmd);

	cmd->viewangles_copy = cmd->viewangles;
	cmd->buttons_copy = cmd->buttons;

	lastCmd = *cmd;

	return silent;
}
