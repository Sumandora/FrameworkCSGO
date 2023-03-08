#include "Hooks.hpp"

#include <csignal>
#include <sys/mman.h>

#include "CreateMove/CreateMoveHook.hpp"
#include "FrameStageNotify/FrameStageNotifyHook.hpp"
#include "SDL/SDLHook.hpp"

#include "Assembly/Assembly.hpp"
#include "Hooking/Hooking.hpp"
#include "Memory/Memory.hpp"

void Hooks::InstallHooks()
{
	FrameStageNotify::Hook();
	CreateMove::Hook();
	SDL::Hook();
}

void Hooks::UninstallHooks()
{
	SDL::Unhook();
	CreateMove::Unhook();
	FrameStageNotify::Unhook();
}

Hook::Hook(void* original, void* hook, int len)
{
	this->original = original;
	this->len = len;
	this->proxy = Framework::Hooking::detour(original, hook, len);
}

Hook::~Hook()
{
	Framework::Memory::protect(original, PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy(original, proxy, len);
	Framework::Memory::protect(original, PROT_READ | PROT_EXEC);
	munmap(static_cast<char*>(proxy) - FRAMEWORK_ABS_JMP_LENGTH, getpagesize());
}
