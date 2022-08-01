#ifndef HOOKS_SDL_HOOK
#define HOOKS_SDL_HOOK

#include <SDL.h>

namespace Hooks {
	namespace SDL {
		inline SDL_Window* windowptr;
		
		inline void* swapWindow_proxy;
		void SDL_GL_SwapWindow_Hook(SDL_Window*);
		
		inline void* pollEvents_proxy;
		int SDL_PollEvents_Hook(SDL_Event*);
		
		void Hook();
	}
}

#endif
