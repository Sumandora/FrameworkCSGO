#include "GameFunctions.hpp"

#include "../../Interfaces.hpp"

#include "BCRL.hpp"

void Hooks::Game::Hook()
{
	CreateMove::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMove::HookFunc));
	FrameStageNotify::hook = new class GameHook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotify::HookFunc));
	OverrideView::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideView::HookFunc));

	// This function references its name through VPROF; Search for ViewDrawFade as string and you will find the function
	ViewDrawFade::hook = new class GameHook(Utils::GetVTable(Interfaces::engineRenderView)[29], reinterpret_cast<void*>(ViewDrawFade::HookFunc));

	// VPROF Function: CGameEventManager::FireEvent
	FireEvent::hook = new class GameHook(
		BCRL::Session::Module("engine_client.so")
			.NextStringOccurence("CGameEventManager::FireEvent")
			.FindXREFs("engine_client.so", true, false)
			.PrevByteOccurence("55 48 89 f8 48 89 e5")
			.Pointer()
			.value(),
		reinterpret_cast<void*>(FireEvent::HookFunc));
}

void Hooks::Game::Unhook()
{
	delete FireEvent::hook;
	delete ViewDrawFade::hook;
	delete OverrideView::hook;
	delete FrameStageNotify::hook;
	delete CreateMove::hook;
}