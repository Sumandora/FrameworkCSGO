#ifndef HOOKS_FRAMESTAGENOTIFY
#define HOOKS_FRAMESTAGENOTIFY

#include "../../../SDK/ClientFrameStage.hpp"
#include "../../../SDK/Math/Matrix4x4.hpp"
#include "../../Hooks.hpp"

namespace Hooks::FrameStageNotify {
	inline Matrix4x4 worldToScreenMatrix{};

	void Hook();
	void Unhook();
}

#endif
