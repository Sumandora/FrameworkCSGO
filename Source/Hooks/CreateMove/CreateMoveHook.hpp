#ifndef HOOKS_CREATEMOVE
#define HOOKS_CREATEMOVE

#include "../../SDK/CUserCmd.hpp"
#include "../Hooks.hpp"

namespace Hooks::CreateMove {
	inline Hook*	 hook;
	inline CUserCmd* lastCmd;

	void			 Hook();
	void			 Unhook();
}

#endif
