#include "SDLFunctions.hpp"

#include <dlfcn.h>

// TODO Mutex

void Hooks::SDL::hook()
{
	void* handle = dlmopen(LM_ID_BASE, "libSDL2-2.0.so.0", RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	DestroyWindow::hook = new SDLHook(handle, "SDL_DestroyWindow", reinterpret_cast<void*>(DestroyWindow::hookFunc));
	WarpMouseInWindow::hook = new SDLHook(handle, "SDL_WarpMouseInWindow", reinterpret_cast<void*>(WarpMouseInWindow::hookFunc));
	SwapWindow::hook = new SDLHook(handle, "SDL_GL_SwapWindow", reinterpret_cast<void*>(SwapWindow::hookFunc));
	PollEvent::hook = new SDLHook(handle, "SDL_PollEvent", reinterpret_cast<void*>(PollEvent::hookFunc));
	dlclose(handle);
}

void Hooks::SDL::unhook()
{
	delete PollEvent::hook;
	delete SwapWindow::hook;
	delete WarpMouseInWindow::hook;
	delete DestroyWindow::hook;
}