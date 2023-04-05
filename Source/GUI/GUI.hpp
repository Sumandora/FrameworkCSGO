#ifndef GUI
#define GUI

#include "SDL.h"

namespace Gui {
	extern bool visible;

	extern float fontSize;
	void RebuildFont();

	void Create();
	void Destroy();

	void SwapWindow(SDL_Window* window);
	void PollEvent(SDL_Event* event);
	bool WarpMouseInWindow();
}

#endif
