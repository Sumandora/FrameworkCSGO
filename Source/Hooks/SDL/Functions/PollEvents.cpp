#include "../SDLFunctions.hpp"

#include "../../../Features/Legit/Aimbot.hpp"

#include "../../../GUI/GUI.hpp"

int Hooks::SDL::PollEvent::HookFunc(SDL_Event* event)
{
	const int returnValue = reinterpret_cast<int (*)(SDL_Event*)>(hook->proxy)(event);

	if (event->type >= SDL_FIRSTEVENT && event->type < SDL_KEYDOWN)
		return returnValue; // These events are not important for our purposes

	Gui::PollEvent(event);
	legitAimbot.PollEvent(event);

	return returnValue;
}
