#include "SDLFunctions.hpp"

// TODO Mutex

void Hooks::SDL::hook()
{
	DestroyWindow::hook = new SDLHook(reinterpret_cast<void*>(SDL_DestroyWindow), reinterpret_cast<void*>(DestroyWindow::hookFunc));
	WarpMouseInWindow::hook = new SDLHook(reinterpret_cast<void*>(SDL_WarpMouseInWindow), reinterpret_cast<void*>(WarpMouseInWindow::hookFunc));
	SwapWindow::hook = new SDLHook(reinterpret_cast<void*>(SDL_GL_SwapWindow), reinterpret_cast<void*>(SwapWindow::hookFunc));
	PollEvent::hook = new SDLHook(reinterpret_cast<void*>(SDL_PollEvent), reinterpret_cast<void*>(PollEvent::hookFunc));
	SetRelativeMouseMode::hook = new SDLHook(reinterpret_cast<void*>(SDL_SetRelativeMouseMode), reinterpret_cast<void*>(SetRelativeMouseMode::hookFunc));
}

void Hooks::SDL::unhook()
{
	delete SetRelativeMouseMode::hook;
	delete PollEvent::hook;
	delete SwapWindow::hook;
	delete WarpMouseInWindow::hook;
	delete DestroyWindow::hook;
}