#include "SDLFunctions.hpp"

#include "xorstr.hpp"

void Hooks::SDL::Hook()
{
	SwapWindow::hook = new class SDLHook(xorstr_("SDL_GL_SwapWindow"), reinterpret_cast<void*>(SwapWindow::HookFunc));
	PollEvents::hook = new class SDLHook(xorstr_("SDL_PollEvent"), reinterpret_cast<void*>(PollEvents::HookFunc));
	WarpMouseInWindow::hook = new class SDLHook(xorstr_("SDL_WarpMouseInWindow"), reinterpret_cast<void*>(WarpMouseInWindow::HookFunc));
}

void Hooks::SDL::Unhook()
{
	delete SwapWindow::hook;
	delete PollEvents::hook;
	delete WarpMouseInWindow::hook;
}