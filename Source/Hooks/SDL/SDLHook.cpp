#include "Hooking/Hooking.hpp"
#include "SDLHook.hpp"

#include "../../GUI.hpp"

#include <cstdio>
#include <dlfcn.h>

void Hooks::SDL::SDL_GL_SwapWindow_Hook(SDL_Window* window) {
	Gui::SwapWindow(window);
	// We are we returning a void? Ah who cares ^^
	return reinterpret_cast<void(*)(SDL_Window*)>(swapWindow_proxy)(window);
}

int Hooks::SDL::SDL_PollEvents_Hook(SDL_Event* event) {
	int returnValue = reinterpret_cast<int(*)(SDL_Event*)>(pollEvents_proxy)(event);

	Gui::PollEvent(event, returnValue);
	
	return returnValue;
}

void Hooks::SDL::SDL_WarpMouseInWindow_Hook(SDL_Window* window, int x, int y) {
	if(Gui::WarpMouseInWindow())
		return;
	
	// Returning voids once again
	return reinterpret_cast<void(*)(SDL_Window*,int,int)>(warpMouseInWindow_proxy)(window, x, y);
}

void* HookSDLFunction(const char* name, void* hook) {
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and let swap it with our method
	 */
	char* address = reinterpret_cast<char*>(dlsym(RTLD_NEXT, name)) + 1;
	void** jumpAddress = reinterpret_cast<void**>(address + *reinterpret_cast<int*>(address + 1) + 5);
	void* realFunction = *jumpAddress;
	*jumpAddress = reinterpret_cast<unsigned int*>(hook);
	return realFunction;
}

void Hooks::SDL::Hook() {
	swapWindow_proxy		= HookSDLFunction("SDL_GL_SwapWindow", reinterpret_cast<void*>(SDL_GL_SwapWindow_Hook));
	pollEvents_proxy		= HookSDLFunction("SDL_PollEvent", reinterpret_cast<void*>(SDL_PollEvents_Hook));
	warpMouseInWindow_proxy	= HookSDLFunction("SDL_WarpMouseInWindow", reinterpret_cast<void*>(SDL_WarpMouseInWindow_Hook));
}
