#ifndef MEMORY
#define MEMORY

#include "SDK/GameClass/CGlobalVars.hpp"

namespace Memory {
	inline void*		ret_instruction_addr;
	inline void*		clientMode;
	inline CGlobalVars* globalVars;

	void  Create();
	void* RelativeToAbsolute(void* addr);
}

#endif
