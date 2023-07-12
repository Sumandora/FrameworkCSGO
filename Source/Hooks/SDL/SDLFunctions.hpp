#ifndef HOOKS_SDL_SDLFUNCTIONS
#define HOOKS_SDL_SDLFUNCTIONS

#include "SDLHook.hpp"

#include "SDL.h"

namespace Hooks::SDL {
	inline SDL_Window* windowPtr = nullptr;
	inline bool shuttingDown = false;

	namespace SwapWindow {
		inline SDLHook* hook;

		void HookFunc(SDL_Window* window);
	}

	namespace PollEvent {
		inline SDLHook* hook;

		int HookFunc(SDL_Event* event);
	}

	namespace WarpMouseInWindow {
		inline SDLHook* hook;

		void HookFunc(SDL_Window* window, int x, int y);
	}

	namespace DestroyWindow {
		inline SDLHook* hook;

		void HookFunc(SDL_Window* window);
	}

	void Hook();
	void Unhook();
}

#endif