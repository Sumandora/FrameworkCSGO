#include "Hooks.hpp"

#include "Game/GameFunctions.hpp"
#include "SDL/SDLFunctions.hpp"

void Hooks::InstallHooks()
{
	Game::Hook();
	SDL::Hook();
}

void Hooks::UninstallHooks()
{
	SDL::Unhook();
	Game::Unhook();
}
