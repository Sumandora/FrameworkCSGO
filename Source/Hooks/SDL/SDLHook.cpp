#include "SDLHook.hpp"

#include "xorstr.hpp"

#include <cstdint>
#include <dlfcn.h>

#include "../../Memory.hpp"

SDLHook::SDLHook(void* handle, const char* name, void* hook)
{
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and swap it with our method
	 */
	void* function = dlsym(handle, name);
	char* relTarget = reinterpret_cast<char*>(function) + 2;
	jumpTarget = reinterpret_cast<void**>(Memory::RelativeToAbsolute(relTarget));
	proxy = *jumpTarget;
	*jumpTarget = reinterpret_cast<uint32_t*>(hook);
}

SDLHook::~SDLHook()
{
	*jumpTarget = proxy;
}