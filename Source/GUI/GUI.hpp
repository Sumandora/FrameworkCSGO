#ifndef GUI
#define GUI

#include "SDL.h"

namespace Gui {
	extern bool visible;

	void create();
	void destroy();

	void build();

	void swapWindow(SDL_Window* window);
	bool pollEvent(SDL_Event* event);
	bool warpMouseInWindow();
}

#endif
