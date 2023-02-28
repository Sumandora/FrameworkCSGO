#ifndef GAMECACHE
#define GAMECACHE

#include "SDK/GameClass/CBasePlayer.hpp"

namespace GameCache {
	CBasePlayer* GetLocalPlayer();
	void		 ClearLocalPlayer();
}

#endif
