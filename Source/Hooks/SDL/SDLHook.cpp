#include <dlfcn.h>

#include "SDLHook.hpp"
#include "xorstr.hpp"

#include "../../Features/Legit/Aimbot.hpp"
#include "../../GUI/GUI.hpp"
#include "../../Memory.hpp"

SDLHook::SDLHook(const char* name, void* hook) {
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and swap it with our method
	 */
	char* address = reinterpret_cast<char*>(dlsym(RTLD_NEXT, name)) + 2;
	jumpAddress	  = reinterpret_cast<void**>(Memory::RelativeToAbsolute(address));
	proxy		  = *jumpAddress;
	*jumpAddress  = reinterpret_cast<unsigned int*>(hook);
}

SDLHook::~SDLHook() {
	*jumpAddress = proxy;
}

void SDL_GL_SwapWindowHook(SDL_Window* window) {
	Hooks::SDL::windowPtr = window;
	Gui::SwapWindow(window);

	return reinterpret_cast<void (*)(SDL_Window*)>(Hooks::SDL::swapWindow->proxy)(window);
}

int SDL_PollEventsHook(SDL_Event* event) {
	int returnValue = reinterpret_cast<int (*)(SDL_Event*)>(Hooks::SDL::pollEvents->proxy)(event);

	Gui::PollEvent(event);
	Features::Legit::Aimbot::PollEvent(event);

	return returnValue;
}

void SDL_WarpMouseInWindowHook(SDL_Window* window, int x, int y) {
	if (Gui::WarpMouseInWindow())
		return;

	return reinterpret_cast<void (*)(SDL_Window*, int, int)>(Hooks::SDL::warpMouseInWindow->proxy)(window, x, y);
}

void Hooks::SDL::Hook() {
	swapWindow		  = new class SDLHook(xorstr_("SDL_GL_SwapWindow"), reinterpret_cast<void*>(SDL_GL_SwapWindowHook));
	pollEvents		  = new class SDLHook(xorstr_("SDL_PollEvent"), reinterpret_cast<void*>(SDL_PollEventsHook));
	warpMouseInWindow = new class SDLHook(xorstr_("SDL_WarpMouseInWindow"), reinterpret_cast<void*>(SDL_WarpMouseInWindowHook));
}

void Hooks::SDL::Unhook() {
	delete swapWindow;
	delete pollEvents;
	delete warpMouseInWindow;
}