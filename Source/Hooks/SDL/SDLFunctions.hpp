#ifndef HOOKS_SDL_SDLFUNCTIONS
#define HOOKS_SDL_SDLFUNCTIONS

#include "SDLHook.hpp"

#include "SDL.h"

namespace Hooks::SDL {
	inline SDL_Window* windowPtr = nullptr;
	inline bool shuttingDown = false;

	inline struct BackupVariable {
		SDL_bool enabled = SDL_GetRelativeMouseMode();
		~BackupVariable() { SDL_SetRelativeMouseMode(enabled); }

		inline SDL_bool& operator=(SDL_bool other) {
			enabled = other;
			return enabled;
		}
		inline operator SDL_bool() const {
			return enabled;
		}
	} relativeMouseMode;

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

	namespace SetRelativeMouseMode {
		inline SDLHook* hook;

		int hookFunc(SDL_bool enabled);
	}

	void hook();
	void unhook();
}

#endif