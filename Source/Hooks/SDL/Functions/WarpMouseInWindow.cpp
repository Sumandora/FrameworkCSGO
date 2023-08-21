#include "../SDLFunctions.hpp"

#include "../../../GUI/GUI.hpp"

void Hooks::SDL::WarpMouseInWindow::hookFunc(SDL_Window* window, int x, int y)
{
	if (Gui::warpMouseInWindow())
		return;

	return reinterpret_cast<void (*)(SDL_Window*, int, int)>(hook->proxy)(window, x, y);
}