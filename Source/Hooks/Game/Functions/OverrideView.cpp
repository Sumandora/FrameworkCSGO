#include "../GameFunctions.hpp"

#include "../../../Features/Visuals/FOV.hpp"

void Hooks::Game::OverrideView::hookFunc(void* thisptr, CViewSetup* pSetup)
{
	invokeFunction<void, void*, CViewSetup*>(hook->proxy, thisptr, pSetup);

	fov.overrideView(pSetup);

	lastViewSetup = *pSetup;
}
