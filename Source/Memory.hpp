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

	void* GetBaseAddress(const char* name);
	void* RelativeToAbsolute(void* addr);

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
}

#endif
