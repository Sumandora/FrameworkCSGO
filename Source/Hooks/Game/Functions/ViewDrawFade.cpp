#include "../GameFunctions.hpp"

#include "../../../Features/Visuals/FlashReduction.hpp"

void Hooks::Game::ViewDrawFade::hookFunc(void* thisptr, std::byte* color, IMaterial* pFadeMaterial, bool mapFullTextureToScreen)
{
	if (flashReduction.viewDrawFade(color, pFadeMaterial))
		return; // Don't draw

	invokeFunction<void, void*, std::byte*, void*, bool>(hook->proxy, thisptr, color, pFadeMaterial, mapFullTextureToScreen);
}
