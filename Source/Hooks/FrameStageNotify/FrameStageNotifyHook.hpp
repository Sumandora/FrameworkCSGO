#ifndef HOOKS_FRAMESTAGENOTIFY
#define HOOKS_FRAMESTAGENOTIFY

#include "../../SDK/Math/Matrix4x4.hpp"
#include "../../SDK/ClientFrameStage.hpp"

namespace Hooks {
	namespace FrameStageNotify {
		inline Matrix4x4 worldToScreenMatrix;
		
		inline void* proxy;
		void FrameStageNotifyHook(void*, ClientFrameStage);
		void Hook();
		void Unhook();
	}
}

#endif
