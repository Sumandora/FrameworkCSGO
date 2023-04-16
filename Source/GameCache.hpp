#ifndef GAMECACHE
#define GAMECACHE

#include "SDK/Math/Vector.hpp"
class CBasePlayer;

namespace GameCache {
	CBasePlayer* GetLocalPlayer();
	void ClearLocalPlayer();

	inline Vector cameraPosition { 0, 0, 0 };
}

#endif
