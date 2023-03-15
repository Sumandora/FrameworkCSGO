#include "Prediction.hpp"

#include "../GameCache.hpp"
#include "../Interfaces.hpp"
#include "../Memory.hpp"

float oldCurtime;
float oldFrametime;

bool Utils::StartPrediction(CUserCmd* cmd, CMoveData& moveData)
{
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (!localPlayer)
		return false;

	oldCurtime = Memory::globalVars->curtime;
	oldFrametime = Memory::globalVars->frametime;

	Memory::globalVars->curtime = static_cast<float>(*localPlayer->TickBase()) * Memory::globalVars->interval_per_tick;
	Memory::globalVars->frametime = Memory::globalVars->interval_per_tick;

	Interfaces::gameMovement->StartTrackpredictionErrors(localPlayer);

	Memory::moveHelper->SetHost(localPlayer);
	Interfaces::prediction->SetupMove(localPlayer, cmd, Memory::moveHelper, &moveData);
	Interfaces::gameMovement->ProcessMovement(localPlayer, &moveData);
	Interfaces::prediction->FinishMove(localPlayer, cmd, &moveData);

	return true;
}

void Utils::EndPrediction()
{
	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (!localPlayer)
		return;

	Interfaces::gameMovement->FinishTrackPredictionErrors(localPlayer);
	Memory::moveHelper->SetHost(nullptr);

	Memory::globalVars->curtime = oldCurtime;
	Memory::globalVars->frametime = oldFrametime;
}
