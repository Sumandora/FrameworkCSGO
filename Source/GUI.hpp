#ifndef GUI
#define GUI

#include <SDL.h>

namespace Gui {
	void Create();
	void SwapWindow(SDL_Window* window);
	void PollEvent(SDL_Event* event, int result);
	bool WarpMouseInWindow();
};

#endif
