#ifndef HOOKS_SDL
#define HOOKS_SDL

#include "SDL.h"

class SDLHook {
	void** jumpAddress;

public:
	void* proxy;

	SDLHook() = delete;
	SDLHook(const char* name, void* hook);
	~SDLHook();
};

namespace Hooks::SDL {
	inline SDLHook* swapWindow = nullptr;
	inline SDLHook* pollEvents = nullptr;
	inline SDLHook* warpMouseInWindow = nullptr;
	inline SDL_Window* windowPtr = nullptr;

	void Hook();
	void Unhook();
}

#endif
