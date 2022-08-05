#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"
#include "Memory/Memory.hpp"
#include "Assembly/Assembly.hpp"

#include "FrameStageNotifyHook.hpp"

#include "../../Interfaces.hpp"

#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdio>

void* frameStageNotify;

void __attribute((optimize("O0"))) Hooks::FrameStageNotify::FrameStageNotifyHook(void* thisptr, ClientFrameStage stage) {
	if(stage == ClientFrameStage::FRAME_START)
		worldToScreenMatrix = Interfaces::engine->WorldToScreenMatrix();
	printf("Updated\n");
	
	return reinterpret_cast<void(*)(void*, ClientFrameStage)>(proxy)(thisptr, stage);
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
