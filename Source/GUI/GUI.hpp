#ifndef GUI
#define GUI

#include "SDL.h"

namespace Gui {
	extern bool visible;

	void Create();
	void Destroy();

	void Build();

	void SwapWindow(SDL_Window* window);
	bool PollEvent(SDL_Event* event);
	bool WarpMouseInWindow();
}

#endif
