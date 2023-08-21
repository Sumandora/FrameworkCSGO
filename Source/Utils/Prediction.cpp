#include "Prediction.hpp"

#include "../Interfaces.hpp"

// TODO Support predicting multiple ticks into the future
float oldCurtime;
float oldFrametime;

bool Utils::startPrediction(CUserCmd* cmd, CMoveData& moveData)
{
	CBasePlayer* localPlayer = Memory::getLocalPlayer();

	if (!localPlayer)
		return false;

	oldCurtime = Memory::globalVars->curtime;
	oldFrametime = Memory::globalVars->frametime;

	Memory::globalVars->curtime = (float)*localPlayer->TickBase() * Memory::globalVars->interval_per_tick;
	Memory::globalVars->frametime = Memory::globalVars->interval_per_tick;

	Interfaces::gameMovement->StartTrackPredictionErrors(localPlayer);

	Memory::moveHelper->SetHost(localPlayer);
	Interfaces::prediction->SetupMove(localPlayer, cmd, Memory::moveHelper, &moveData);
	Interfaces::gameMovement->ProcessMovement(localPlayer, &moveData);
	Interfaces::prediction->FinishMove(localPlayer, cmd, &moveData);

	return true;
}

void Utils::endPrediction()
{
	CBasePlayer* localPlayer = Memory::getLocalPlayer();

	if (!localPlayer)
		return;

	Interfaces::gameMovement->FinishTrackPredictionErrors(localPlayer);
	Memory::moveHelper->SetHost(nullptr);

	Memory::globalVars->curtime = oldCurtime;
	Memory::globalVars->frametime = oldFrametime;
}
