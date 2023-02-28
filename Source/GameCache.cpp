#include "GameCache.hpp"
#include "Interfaces.hpp"

static CBasePlayer* localPlayer;

CBasePlayer* GameCache::GetLocalPlayer() {
	if (!localPlayer) {
		localPlayer = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(Interfaces::engine->GetLocalPlayer()));
	}
	return localPlayer;
}

void GameCache::ClearLocalPlayer() {
	localPlayer = nullptr;
}