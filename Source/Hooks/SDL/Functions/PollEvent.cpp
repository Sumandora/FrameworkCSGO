#include "../SDLFunctions.hpp"

#include "../../../Features/Legit/Aimbot.hpp"

#include "../../../GUI/GUI.hpp"

int Hooks::SDL::PollEvent::hookFunc(SDL_Event* event)
{
	int returnValue = reinterpret_cast<int (*)(SDL_Event*)>(hook->proxy)(event);

	if (event->type >= SDL_FIRSTEVENT && event->type < SDL_KEYDOWN)
		return returnValue; // These events are not important for our purposes

	while (Gui::pollEvent(event) && returnValue > 0 /* There are events available */) { // Regenerate events until we find one, which is allowed to pass
		returnValue = reinterpret_cast<int (*)(SDL_Event*)>(hook->proxy)(event);
	}
	legitAimbot.pollEvent(event);

	return returnValue;
}
