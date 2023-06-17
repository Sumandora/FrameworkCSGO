#include "SDLFunctions.hpp"

#include "xorstr.hpp"

#include <dlfcn.h>

void Hooks::SDL::Hook()
{
	void* handle = dlmopen(LM_ID_BASE, xorstr_("libSDL2-2.0.so.0"), RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	SwapWindow::hook = new class SDLHook(handle, xorstr_("SDL_GL_SwapWindow"), reinterpret_cast<void*>(SwapWindow::HookFunc));
	PollEvents::hook = new class SDLHook(handle, xorstr_("SDL_PollEvent"), reinterpret_cast<void*>(PollEvents::HookFunc));
	WarpMouseInWindow::hook = new class SDLHook(handle, xorstr_("SDL_WarpMouseInWindow"), reinterpret_cast<void*>(WarpMouseInWindow::HookFunc));
	dlclose(handle);
}

void Hooks::SDL::Unhook()
{
	delete WarpMouseInWindow::hook;
	delete PollEvents::hook;
	delete SwapWindow::hook;
}