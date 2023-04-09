#include "OverrideViewHook.hpp"

#include "../../Features/Features.hpp"

void OverrideViewHook(void* thisptr, CViewSetup* pSetup)
{
	invokeFunction<void, void*, CViewSetup*>(Hooks::OverrideView::hook->proxy, thisptr, pSetup);

	Features::Visuals::FOV::OverrideView(pSetup);

	Hooks::OverrideView::lastViewSetup = *pSetup;
}

void Hooks::OverrideView::Hook()
{
	hook = new class Hook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideViewHook));
}

void Hooks::OverrideView::Unhook()
{
	delete hook;
}
