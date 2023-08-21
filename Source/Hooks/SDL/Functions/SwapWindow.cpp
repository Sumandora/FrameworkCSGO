#include "../SDLFunctions.hpp"

#include "../../../GUI/GUI.hpp"

void Hooks::SDL::SwapWindow::hookFunc(SDL_Window* window)
{
	Hooks::SDL::windowPtr = window;
	Gui::swapWindow(window);

	return reinterpret_cast<void (*)(SDL_Window*)>(hook->proxy)(window);
}