#include "../GameFunctions.hpp"

#include "../../../Features/Visuals/FOV.hpp"

void Hooks::Game::OverrideView::HookFunc(void* thisptr, CViewSetup* pSetup)
{
	InvokeFunction<void, void*, CViewSetup*>(hook->proxy, thisptr, pSetup);

	fov.OverrideView(pSetup);

	lastViewSetup = *pSetup;
}
