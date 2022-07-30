#ifndef HOOKS_CREATEMOVE_HOOK
#define HOOKS_CREATEMOVE_HOOK

#include "../../SDK/CUserCmd.hpp"

namespace Hooks {
	namespace CreateMove {
		inline void* proxy;
		bool CreateMoveHook(void*, float, CUserCmd*);
		void Hook();
		void Unhook();
	}
}

#endif
