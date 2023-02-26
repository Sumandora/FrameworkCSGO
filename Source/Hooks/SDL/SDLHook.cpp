#include <dlfcn.h>

#include "xorstr.hpp"
#include "SDLHook.hpp"

#include "../../Memory.hpp"
#include "../../GUI/GUI.hpp"
#include "../../Features/Legit/Aimbot.hpp"

void Hooks::SDL::SDL_GL_SwapWindow_Hook(SDL_Window* window) {
	windowPtr = window;
	Gui::SwapWindow(window);

	return reinterpret_cast<void(*)(SDL_Window*)>(swapWindow_proxy)(window);
}

int Hooks::SDL::SDL_PollEvents_Hook(SDL_Event* event) {
	int returnValue = reinterpret_cast<int(*)(SDL_Event*)>(pollEvents_proxy)(event);

	Gui::PollEvent(event, returnValue);
	Features::Legit::Aimbot::PollEvent(event);
	
	return returnValue;
}

void Hooks::SDL::SDL_WarpMouseInWindow_Hook(SDL_Window* window, int x, int y) {
	if(Gui::WarpMouseInWindow())
		return;
	
	return reinterpret_cast<void(*)(SDL_Window*,int,int)>(warpMouseInWindow_proxy)(window, x, y);
}

void* HookSDLFunction(const char* name, void* hook) {
	/**
	 * These are wrapper functions
	 * They have a relative jmp instruction inside of them.
	 * We take the address of the relative jmp and swap it with our method
	 */
	char* address = reinterpret_cast<char*>(dlsym(RTLD_NEXT, name)) + 2;
	void** jumpAddress = reinterpret_cast<void**>(Memory::RelativeToAbsolute(address));
	void* realFunction = *jumpAddress;
	*jumpAddress = reinterpret_cast<unsigned int*>(hook);
	return realFunction;
}

void Hooks::SDL::Hook() {
	swapWindow_proxy		= HookSDLFunction(xorstr_("SDL_GL_SwapWindow"),		reinterpret_cast<void*>(SDL_GL_SwapWindow_Hook));
	pollEvents_proxy		= HookSDLFunction(xorstr_("SDL_PollEvent"),			reinterpret_cast<void*>(SDL_PollEvents_Hook));
	warpMouseInWindow_proxy	= HookSDLFunction(xorstr_("SDL_WarpMouseInWindow"),	reinterpret_cast<void*>(SDL_WarpMouseInWindow_Hook));
}

void Hooks::SDL::Unhook() {
	HookSDLFunction(xorstr_("SDL_GL_SwapWindow"),		swapWindow_proxy);
	HookSDLFunction(xorstr_("SDL_PollEvent"),			pollEvents_proxy);
	HookSDLFunction(xorstr_("SDL_WarpMouseInWindow"),	warpMouseInWindow_proxy);
}
