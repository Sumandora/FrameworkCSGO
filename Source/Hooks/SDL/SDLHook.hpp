#ifndef HOOKS_SDL
#define HOOKS_SDL

#include "SDL.h"

class SDLHook {
	void** jumpAddress;

public:
	void* proxy;

	SDLHook(const char* name, void* hook);
	~SDLHook();
};

namespace Hooks::SDL {
	inline SDLHook*	   swapWindow;
	inline SDLHook*	   pollEvents;
	inline SDLHook*	   warpMouseInWindow;
	inline SDL_Window* windowPtr;

	void Hook();
	void Unhook();
}

#endif
