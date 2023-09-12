#include "../SDLFunctions.hpp"

#include "../../../GUI/GUI.hpp"

int Hooks::SDL::SetRelativeMouseMode::hookFunc(SDL_bool enabled)
{
	relativeMouseMode = enabled;
	if(!Gui::visible)
		return reinterpret_cast<int (*)(SDL_bool enabled)>(hook->proxy)(enabled);
	return 0; // :thumbsup:
}