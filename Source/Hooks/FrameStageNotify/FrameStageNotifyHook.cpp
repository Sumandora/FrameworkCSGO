#include "FrameStageNotifyHook.hpp"

#include "../../Interfaces.hpp"

#include "../../Features/Semirage/Backtrack.hpp"

void __attribute((optimize("O0"))) FrameStageNotifyHook(void* thisptr, ClientFrameStage stage) {
	switch (stage) {
		case ClientFrameStage::FRAME_RENDER_END: {
			Hooks::FrameStageNotify::worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
			break;
		}
		case ClientFrameStage::FRAME_RENDER_START: {
			Features::Semirage::Backtrack::FrameStageNotify();
		}
		default:
			// ignored
			break;
	}
	return Framework::ReturnAddr::invoke<void, void*, ClientFrameStage>(Hooks::FrameStageNotify::hook->proxy, Memory::ret_instruction_addr, thisptr, stage);
}

void Hooks::FrameStageNotify::Hook() {
	hook = new class Hook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotifyHook), 6);
}

void Hooks::FrameStageNotify::Unhook() {
	delete hook;
}
