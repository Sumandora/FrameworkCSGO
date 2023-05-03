#ifndef MEMORY
#define MEMORY

class CClientMode;
class IMoveHelper;
#include "SDK/GameClass/CGlobalVars.hpp"
class CUserCmd;
struct Vector;

namespace Memory {
	inline CClientMode* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void* RelativeToAbsolute(void* addr);

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
}

#endif
