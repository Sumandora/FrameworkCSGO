#include "FrameStageNotifyHook.hpp"

#include "../../Interfaces.hpp"

#include "../../Features/Semirage.hpp"

void FrameStageNotifyHook(void* thisptr, ClientFrameStage stage)
{
	switch (stage) {
	case ClientFrameStage::FRAME_RENDER_START: {
		Features::Semirage::Backtrack::FrameStageNotify();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_END: {
		Hooks::FrameStageNotify::worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
		break;
	}
	default:
		// ignored
		break;
	}
	return invokeFunction<void, void*, ClientFrameStage>(Hooks::FrameStageNotify::hook->proxy, thisptr, stage);
}

void Hooks::FrameStageNotify::Hook()
{
	hook = new class Hook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotifyHook), 6);
}

void Hooks::FrameStageNotify::Unhook()
{
	delete hook;
}
