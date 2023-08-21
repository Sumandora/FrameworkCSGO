#ifndef HOOKS_SDL_SDLFUNCTIONS
#define HOOKS_SDL_SDLFUNCTIONS

#include "SDLHook.hpp"

#include "SDL.h"

namespace Hooks::SDL {
	inline SDL_Window* windowPtr = nullptr;
	inline bool shuttingDown = false;

	namespace SwapWindow {
		inline SDLHook* hook;

		void hookFunc(SDL_Window* window);
	}

	namespace PollEvent {
		inline SDLHook* hook;

		int hookFunc(SDL_Event* event);
	}

	namespace WarpMouseInWindow {
		inline SDLHook* hook;

		void hookFunc(SDL_Window* window, int x, int y);
	}

	namespace DestroyWindow {
		inline SDLHook* hook;

		void hookFunc(SDL_Window* window);
	}

	void hook();
	void unhook();
}

#endif