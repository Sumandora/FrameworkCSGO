#include "../GameFunctions.hpp"

#include <cstdio>
#include <cstring>

#include "../../../Features/Misc/IgnoreSvPure.hpp"

bool Hooks::Game::CanLoadThirdPartyFiles::HookFunc(void* thisptr)
{
	if (ignoreSvPure.allowThirdPartyFiles)
		return true; // "Abgenickt von Tech-Nick"
	return InvokeFunction<bool, void*>(hook->proxy, thisptr);
}
