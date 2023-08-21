#include "../GameFunctions.hpp"

#include <cstdio>
#include <cstring>

#include "../../../Features/Misc/IgnoreSvPure.hpp"

bool Hooks::Game::CanLoadThirdPartyFiles::hookFunc(void* thisptr)
{
	if (ignoreSvPure.allowThirdPartyFiles)
		return true; // "Abgenickt von Tech-Nick"
	return invokeFunction<bool, void*>(hook->proxy, thisptr);
}
