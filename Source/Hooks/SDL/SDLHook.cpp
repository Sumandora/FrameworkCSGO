#include "Hooking/Hooking.hpp"
#include "SDLHook.hpp"

#include "../../Features/Legit/Aimbot.hpp"
#include "backends/imgui_impl_sdl.h"

#include <cstdio>
#include <dlfcn.h>

int Hooks::SDL::SDL_PollEvents_Hook(SDL_Event* event) {
	int returnValue = reinterpret_cast<int(*)(SDL_Event*)>(pollEvents_proxy)(event);
	if (!windowptr && event->type == SDL_WINDOWEVENT) {
		SDL_Window* pWindow = SDL_GetWindowFromID(event->window.windowID);
		if (pWindow) {
			windowptr = pWindow;
		}
	}
	if (ImGui::GetCurrentContext( )) {
		ImGui_ImplSDL2_ProcessEvent(event);
	}
		
	// Features::Legit::Aimbot::PollEvent(event);
	
	return returnValue;
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
	pollEvents_proxy	= HookSDLFunction("SDL_PollEvent", reinterpret_cast<void*>(SDL_PollEvents_Hook));
}
