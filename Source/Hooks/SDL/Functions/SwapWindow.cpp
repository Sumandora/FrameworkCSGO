#include "Hooks/SDL/SDLFunctions.hpp"

#include "GUI/GUI.hpp"

void Hooks::SDL::SwapWindow::HookFunc(SDL_Window* window)
{
	Hooks::SDL::windowPtr = window;
	Gui::SwapWindow(window);

	return reinterpret_cast<void (*)(SDL_Window*)>(hook->proxy)(window);
}