#include "FrameStageNotifyHook.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <cstring>

#include "Hooking/Hooking.hpp"
#include "Memory/Memory.hpp"
#include "Assembly/Assembly.hpp"
#include "ReturnAddr/ReturnAddr.hpp"

#include "../../Interfaces.hpp"
#include "../../Memory.hpp"

#include "../../SDK/ClientFrameStage.hpp"

#include "../../SDK/GameClass/Interfaces/CEngineClient.hpp"

#include "../../SDK/Math/Matrix4x4.hpp"

#include "../../Utils/VMT.hpp"

void* frameStageNotify;

void __attribute((optimize("O0"))) Hooks::FrameStageNotify::FrameStageNotifyHook(void* thisptr, ClientFrameStage stage) {
	if(stage == ClientFrameStage::FRAME_START) {
		worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
	}
	return Framework::ReturnAddr::invoke<void, void*, ClientFrameStage>(proxy, Memory::ret_instruction_addr, thisptr, stage);
}

void Hooks::FrameStageNotify::Hook() {	
	frameStageNotify = Utils::GetVTable(Interfaces::baseClient)[37];

	proxy = Framework::Hooking::detour(frameStageNotify, reinterpret_cast<void*>(FrameStageNotifyHook), 6);
}

void Hooks::FrameStageNotify::Unhook() {
	Framework::Memory::protect(frameStageNotify, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(frameStageNotify, proxy, 6);
	Framework::Memory::protect(frameStageNotify, PROT_READ | PROT_EXEC);
	munmap(static_cast<char*>(frameStageNotify) - FRAMEWORK_ABS_JMP_LENGTH, getpagesize());
}
