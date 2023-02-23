#ifndef HOOKS_PAINT_HOOK
#define HOOKS_PAINT_HOOK

#include "../../SDK/Math/Matrix4x4.hpp"
#include "../../SDK/ClientFrameStage.hpp"

enum class ClientFrameStage : int;

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
