#include "../GameFunctions.hpp"

#include "../../../Features/Visuals/Glow/Glow.hpp"

bool Hooks::Game::DoPostScreenEffects::hookFunc(void* thisptr, void* rsi) // rsi might be the render context, however I don't care nor do I need to know what rsi really is here
{
	glow.updateGlow();
	return invokeFunction<bool, void*, void*>(hook->proxy, thisptr, rsi);
}
