#ifndef GAMECACHE
#define GAMECACHE

#include "SDK/Math/Vector.hpp"
class CBasePlayer;

namespace GameCache {
	CBasePlayer* GetLocalPlayer();
	void ClearLocalPlayer();
}

#endif
