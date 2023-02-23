#ifndef MEMORY
#define MEMORY

#include "SDK/GameClass/CGlobalVars.hpp"

class CGlobalVars;

namespace Memory {
	inline void*		ret_instruction_addr;
	inline void*		clientMode;
	inline CGlobalVars*	globalVars;

	void Create();
}

#endif
