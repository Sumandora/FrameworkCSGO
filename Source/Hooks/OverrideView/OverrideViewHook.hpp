#ifndef HOOKS_OVERRIDEVIEW
#define HOOKS_OVERRIDEVIEW

#include "../Hooks.hpp"
#include "../../SDK/GameClass/CViewSetup.hpp"

namespace Hooks::OverrideView {
	inline Hook* hook;
	inline CViewSetup lastViewSetup;

	void Hook();
	void Unhook();
}

#endif
