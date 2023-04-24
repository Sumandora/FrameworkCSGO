#include "OverrideViewHook.hpp"

#include "../../../Features/Features.hpp"
#include "../../../Interfaces.hpp"

#include "../GameHook.hpp"

static GameHook* hook;

void OverrideViewHook(void* thisptr, CViewSetup* pSetup)
{
	InvokeFunction<void, void*, CViewSetup*>(hook->proxy, thisptr, pSetup);

	if (Interfaces::engine->IsInGame())
		Hooks::OverrideView::cameraPosition = pSetup->origin;

	Features::Visuals::FOV::OverrideView(pSetup);

	Hooks::OverrideView::lastViewSetup = *pSetup;
}

void Hooks::OverrideView::Hook()
{
	hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideViewHook));
}

void Hooks::OverrideView::Unhook()
{
	delete hook;
}
