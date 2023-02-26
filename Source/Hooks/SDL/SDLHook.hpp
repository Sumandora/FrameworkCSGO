#ifndef HOOKS_SDL
#define HOOKS_SDL

#include "SDL.h"

namespace Hooks {
	namespace SDL {
		inline SDL_Window* windowPtr;

		inline void* swapWindow_proxy;
		void SDL_GL_SwapWindow_Hook(SDL_Window*);
		
		inline void* pollEvents_proxy;
		int SDL_PollEvents_Hook(SDL_Event*);
		
		inline void* warpMouseInWindow_proxy;
		void SDL_WarpMouseInWindow_Hook(SDL_Window*, int, int);
		
		void Hook();
		void Unhook();
	}
}

#endif
