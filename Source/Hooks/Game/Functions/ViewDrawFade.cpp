#include "../GameFunctions.hpp"

#include "../../../Features/Visuals/FlashReduction.hpp"

void Hooks::Game::ViewDrawFade::HookFunc(void* thisptr, std::byte* color, IMaterial* pFadeMaterial, bool mapFullTextureToScreen)
{
	if (flashReduction.ViewDrawFade(color, pFadeMaterial))
		return; // Don't draw

	InvokeFunction<void, void*, std::byte*, void*, bool>(hook->proxy, thisptr, color, pFadeMaterial, mapFullTextureToScreen);
}
