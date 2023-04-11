#include "Hooks.hpp"

#include <csignal>
#include <sys/mman.h>

#include "CreateMove/CreateMoveHook.hpp"
#include "FrameStageNotify/FrameStageNotifyHook.hpp"
#include "OverrideView/OverrideViewHook.hpp"
#include "SDL/SDLHook.hpp"

#include "Assembly/Assembly.hpp"
#include "Hooking/Hooking.hpp"
#include "Memory/Memory.hpp"

#include "ldisasm.h"

void Hooks::InstallHooks()
{
	FrameStageNotify::Hook();
	CreateMove::Hook();
	OverrideView::Hook();
	SDL::Hook();
}

void Hooks::UninstallHooks()
{
	SDL::Unhook();
	OverrideView::Unhook();
	CreateMove::Unhook();
	FrameStageNotify::Unhook();
}

Hook::Hook(void* original, void* hook)
{
	this->original = original;

	this->len = 0;
	while (this->len <= FRAMEWORK_NEAR_JMP_LENGTH) {
		this->len += ldisasm(static_cast<char*>(original) + this->len, true);
	}

	this->proxy = Framework::Hooking::detour(original, hook, (int)this->len);
}

Hook::~Hook()
{
	Framework::Memory::protect(original, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(original, proxy, len);
	Framework::Memory::protect(original, PROT_READ | PROT_EXEC);
	munmap(static_cast<char*>(proxy) - FRAMEWORK_ABS_JMP_LENGTH, getpagesize());
}
