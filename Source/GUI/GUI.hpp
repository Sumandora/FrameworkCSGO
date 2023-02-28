#ifndef GUI_GUI
#define GUI_GUI

#include "SDL.h"

#include "Elements/Keybind.hpp"

namespace Gui {
	void Create();
	void Destroy();

	void SwapWindow(SDL_Window* window);
	void PollEvent(SDL_Event* event);
	bool WarpMouseInWindow();
}

#endif
