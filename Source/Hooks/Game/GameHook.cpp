#include "GameHook.hpp"

#include "DetourHooking.hpp"
#include "ldisasm.h"

// TODO Alternative hooking methods?

GameHook::GameHook(void* original, void* hook)
{
	size_t len = 0;
	while (len <= DetourHooking::minLength) {
		len += ldisasm(reinterpret_cast<char*>(original) + len, true);
	}
	backingHook = new DetourHooking::Hook(original, hook, len);
	backingHook->Enable();

	proxy = backingHook->trampoline;
}

GameHook::~GameHook()
{
	backingHook->Disable();
	delete backingHook;
}