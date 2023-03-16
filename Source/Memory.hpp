#ifndef MEMORY
#define MEMORY

class IMoveHelper;
#include "SDK/GameClass/CGlobalVars.hpp"
class Vector;

namespace Memory {
	inline void* ret_instruction_addr;
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void Create();
	void* RelativeToAbsolute(void* addr);

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
}

#endif
