#ifndef MEMORY
#define MEMORY

#include "SDK/CUtl/Vector.hpp"
#include "SDK/GameClass/CGlobalVars.hpp"
#include <vector>
class IMoveHelper;
class CBasePlayer;
class CGlowObjectManager;

struct Vector;

namespace Memory {
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;
	inline CGlowObjectManager* glowObjectManager;

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
	CBasePlayer* GetLocalPlayer();
}

#endif
