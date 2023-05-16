#include "SDLHook.hpp"

#include <cstdint>
#include <dlfcn.h>

#include "Memory.hpp"

SDLHook::SDLHook(const char* name, void* hook)
{
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and swap it with our method
	 */
	char* address = reinterpret_cast<char*>(dlsym(RTLD_NEXT, name)) + 2;
	jumpAddress = reinterpret_cast<void**>(Memory::RelativeToAbsolute(address));
	proxy = *jumpAddress;
	*jumpAddress = reinterpret_cast<uint32_t*>(hook);
}

SDLHook::~SDLHook()
{
	*jumpAddress = proxy;
}