#include "OverrideViewHook.hpp"

#include "../../Features/Features.hpp"

void OverrideViewHook(void* thisptr, CViewSetup* pSetup)
{
	invokeFunction<void, void*, CViewSetup*>(Hooks::OverrideView::hook->proxy, thisptr, pSetup);

	Features::Visuals::FOV::OverrideView(pSetup);
}

void Hooks::OverrideView::Hook()
{
	hook = new class Hook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideViewHook), 6);
}

void Hooks::OverrideView::Unhook()
{
	delete hook;
}
