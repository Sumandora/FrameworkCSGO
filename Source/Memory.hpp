#ifndef MEMORY
#define MEMORY

class IMoveHelper;
#include "SDK/GameClass/CGlobalVars.hpp"

namespace Memory {
	inline void* ret_instruction_addr;
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	void Create();
	void* RelativeToAbsolute(void* addr);
}

#endif
