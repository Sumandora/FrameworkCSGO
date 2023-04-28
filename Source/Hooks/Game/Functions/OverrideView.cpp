#include "../GameFunctions.hpp"

#include "../../../Features/Features.hpp"

void Hooks::Game::OverrideView::HookFunc(void* thisptr, CViewSetup* pSetup)
{
	InvokeFunction<void, void*, CViewSetup*>(hook->proxy, thisptr, pSetup);

	Features::Visuals::FOV::OverrideView(pSetup);

	cameraPosition = pSetup->origin;
	lastViewSetup = *pSetup;
}
