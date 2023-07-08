#include "SDLFunctions.hpp"

#include <dlfcn.h>

void Hooks::SDL::Hook()
{
	void* handle = dlmopen(LM_ID_BASE, "libSDL2-2.0.so.0", RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	SwapWindow::hook = new class SDLHook(handle, "SDL_GL_SwapWindow", reinterpret_cast<void*>(SwapWindow::HookFunc));
	PollEvent::hook = new class SDLHook(handle, "SDL_PollEvent", reinterpret_cast<void*>(PollEvent::HookFunc));
	WarpMouseInWindow::hook = new class SDLHook(handle, "SDL_WarpMouseInWindow", reinterpret_cast<void*>(WarpMouseInWindow::HookFunc));
	dlclose(handle);
}

void Hooks::SDL::Unhook()
{
	delete WarpMouseInWindow::hook;
	delete PollEvent::hook;
	delete SwapWindow::hook;
}