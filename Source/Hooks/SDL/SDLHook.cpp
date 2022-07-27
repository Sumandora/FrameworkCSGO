#include "Hooking/Hooking.hpp"
#include "SDLHook.hpp"

#include <cstdio>
#include <dlfcn.h>

void Hooks::SDL::SDL_GL_SwapWindow_Hook(SDL_Window* window) {
	printf("SwapWindow\n");

	// We are we returning a void? Ah who cares ^^
	return reinterpret_cast<void(*)(SDL_Window*)>(swapWindow_proxy)(window);
}

int Hooks::SDL::SDL_PollEvents_Hook(SDL_Event* event) {
	printf("PollEvents\n");
	
	return reinterpret_cast<int(*)(SDL_Event*)>(pollEvents_proxy)(event);
}


void Hooks::SDL::Hook() {
	/**
	 * These are wrapper function
	 * It has a relative jmp instruction inside of it.
	 * We take the address of the relative jmp and let swap it with our method
	 */
	char* swapWindow = reinterpret_cast<char*>(dlsym(RTLD_NEXT, "SDL_GL_SwapWindow")) + 1;
	void** swapWindowJumpAddress = reinterpret_cast<void**>(swapWindow + *reinterpret_cast<int*>(swapWindow + 1) + 5);
	swapWindow_proxy = *swapWindowJumpAddress;
	*swapWindowJumpAddress = reinterpret_cast<unsigned int*>(&SDL_GL_SwapWindow_Hook);
	
	char* pollEvents = reinterpret_cast<char*>(dlsym(RTLD_NEXT, "SDL_PollEvent")) + 1;
	void** pollEventsJumpAddress = reinterpret_cast<void**>(pollEvents + *reinterpret_cast<int*>(pollEvents + 1) + 5);
	pollEvents_proxy = *pollEventsJumpAddress;
	*pollEventsJumpAddress = reinterpret_cast<unsigned int*>(&SDL_PollEvents_Hook);
}
