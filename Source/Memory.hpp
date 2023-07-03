#ifndef MEMORY
#define MEMORY

#include "SDK/GameClass/CGlobalVars.hpp"
class IMoveHelper;
class CBasePlayer;
struct Vector;

namespace Memory {
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
	CBasePlayer* GetLocalPlayer();
}

#endif
