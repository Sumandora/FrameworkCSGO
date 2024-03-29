#include "SDLHook.hpp"

#include "BCRL.hpp"

SDLHook::SDLHook(void* original, void* hook)
{
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and swap it with our method
	 */
	jumpTarget = static_cast<void**>(BCRL::Session::pointer(original).add(2).relativeToAbsolute().getPointer().value());
	proxy = *jumpTarget;
	*jumpTarget = static_cast<uint32_t*>(hook);
}

SDLHook::~SDLHook()
{
	*jumpTarget = proxy;
}