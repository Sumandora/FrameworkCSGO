#include "Hooks/SDL/SDLFunctions.hpp"

#include "GUI/GUI.hpp"

void Hooks::SDL::WarpMouseInWindow::HookFunc(SDL_Window* window, int x, int y)
{
	if (Gui::WarpMouseInWindow())
		return;

	return reinterpret_cast<void (*)(SDL_Window*, int, int)>(hook->proxy)(window, x, y);
}