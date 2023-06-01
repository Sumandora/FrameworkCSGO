#include "GameHook.hpp"

#include "ldisasm.h"

// TODO Alternative hooking methods?

GameHook::GameHook(void* original, void* hook)
{
	size_t len = 0;
	while (len <= DETOURHOOKING_MIN_LENGTH) {
		len += ldisasm(static_cast<char*>(original) + len, true);
	}
	backingHook = new class Hook(original, hook, len);
	backingHook->Enable();

	proxy = backingHook->trampoline;
}

GameHook::~GameHook()
{
	backingHook->Disable();
}