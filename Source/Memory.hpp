#ifndef MEMORY
#define MEMORY

class IMoveHelper;
#include "SDK/GameClass/CGlobalVars.hpp"
struct Vector;

namespace Memory {
	inline void* ret_instruction_addr;
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void* RelativeToAbsolute(void* addr);

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
}

#endif
