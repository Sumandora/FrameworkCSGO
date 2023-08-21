#include "../SDLFunctions.hpp"

void Hooks::SDL::DestroyWindow::hookFunc(SDL_Window* window)
{
	if (Hooks::SDL::windowPtr == window)
		shuttingDown = true;
	return reinterpret_cast<void (*)(SDL_Window*)>(hook->proxy)(window);
}