#ifndef HOOKS_OVERRIDEVIEW
#define HOOKS_OVERRIDEVIEW

#include "../../SDK/GameClass/CViewSetup.hpp"
#include "../Hooks.hpp"

namespace Hooks::OverrideView {
	inline Hook* hook;
	inline CViewSetup lastViewSetup;

	void Hook();
	void Unhook();
}

#endif
