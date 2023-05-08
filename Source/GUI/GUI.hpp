#ifndef GUI
#define GUI

#include "SDL.h"

namespace Gui {
	extern bool visible;

	void Create();
	void Destroy();

	void BuildMenu(int width, int height);

	void SwapWindow(SDL_Window* window);
	void PollEvent(SDL_Event* event);
	bool WarpMouseInWindow();
}

#endif
