#ifndef MEMORY
#define MEMORY

class IMoveHelper;
#include "SDK/GameClass/CGlobalVars.hpp"
#include "SDK/Math/Vector.hpp"

namespace Memory {
	inline void* ret_instruction_addr;
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void Create();
	void* RelativeToAbsolute(void* addr);

	bool LineGoesThroughSmoke(Vector from, Vector to, short _);
}

#endif
