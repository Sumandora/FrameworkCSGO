#ifndef HOOKS_CREATEMOVE
#define HOOKS_CREATEMOVE

#include "../../SDK/CUserCmd.hpp"

namespace Hooks {
	namespace CreateMove {
		inline void* proxy;
		inline CUserCmd* lastCmd;
		bool CreateMoveHook(void*, float, CUserCmd*);
		void Hook();
		void Unhook();
	}
}

#endif
