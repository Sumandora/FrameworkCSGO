#include "FrameStageNotifyHook.hpp"

#include "../../Interfaces.hpp"
#include "../Hooks.hpp"

void __attribute((optimize("O0"))) FrameStageNotifyHook(void* thisptr, ClientFrameStage stage) {
	if (stage == ClientFrameStage::FRAME_START) {
		Hooks::FrameStageNotify::worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
	}
	return Framework::ReturnAddr::invoke<void, void*, ClientFrameStage>(Hooks::FrameStageNotify::hook->proxy, Memory::ret_instruction_addr, thisptr, stage);
}

void Hooks::FrameStageNotify::Hook() {
	hook = new class Hook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotifyHook), 6);
}

void Hooks::FrameStageNotify::Unhook() {
	delete hook;
}
