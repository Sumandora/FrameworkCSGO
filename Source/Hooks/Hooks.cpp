#include <cassert>
#include "Hooks.hpp"

#include "Game/GameFunctions.hpp"
#include "SDL/SDLFunctions.hpp"

#include "HideSharedObject.hpp"

static GameHook* dlIteratePhdr;
void* HideSharedObject::proxy;

void Hooks::InstallHooks()
{
	Game::Hook();
	SDL::Hook();

	HideSharedObject::AddHiddenSharedObject(xorstr_("lib" PROJECT_NAME ".so"));

	dlIteratePhdr = new class GameHook((void*) dl_iterate_phdr, (void*) HideSharedObject::HookFunc);
	HideSharedObject::proxy = dlIteratePhdr->proxy;
}

void Hooks::UninstallHooks()
{
	delete dlIteratePhdr;

	SDL::Unhook();
	Game::Unhook();
}
