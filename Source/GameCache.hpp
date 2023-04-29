#ifndef GAMECACHE
#define GAMECACHE

class CBasePlayer;

namespace GameCache {
	CBasePlayer* GetLocalPlayer();
	void ClearLocalPlayer();
}

#endif
