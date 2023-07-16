#include "GameFunctions.hpp"

#include "../../Interfaces.hpp"

#include "BCRL.hpp"
#include "GameHook.hpp"

void Hooks::Game::Hook()
{
	CreateMove::hook = new GameHook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMove::HookFunc));
	FrameStageNotify::hook = new GameHook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotify::HookFunc));
	OverrideView::hook = new GameHook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideView::HookFunc));

	// This function references its name through VPROF; Search for ViewDrawFade as string and you will find the function
	ViewDrawFade::hook = new GameHook(Utils::GetVTable(Interfaces::engineRenderView)[29], reinterpret_cast<void*>(ViewDrawFade::HookFunc));

	FireEvent::hook = new GameHook(
		BCRL::Session::Module("engine_client.so")
			.NextStringOccurence("CGameEventManager::FireEvent")
			.FindXREFs("engine_client.so", true, false)
			.PrevByteOccurence("55 48 89 f8 48 89 e5")
			.Pointer()
			.value(),
		reinterpret_cast<void*>(FireEvent::HookFunc));

	EmitSound::hook = new GameHook(
		BCRL::Session::Module("engine_client.so")
			.NextStringOccurence("EmitSound: %s pitch out of bounds = %i\n")
			.FindXREFs("engine_client.so", true, false)
			.Filter([](const BCRL::SafePointer& safePointer) { return safePointer.Add(4).Equals('\xe8'); })
			.PrevByteOccurence("55 66 0f ef db")
			.Pointer()
			.value(),
		reinterpret_cast<void*>(EmitSound::HookFunc));

	SendNetMsg::hook = new GameHook(
		BCRL::Session::Module("engine_client.so")
			.NextStringOccurence("SendNetMsg %s: stream[%s] buffer overflow (maxsize = %d)!\n")
			.FindXREFs("engine_client.so", true, false)
			.Filter([](const BCRL::SafePointer& safePointer) { return safePointer.Add(4).DoesMatch("44 89 e9"); })
			.PrevByteOccurence("55 48 89 e5 41 57")
			.Pointer()
			.value(),
		reinterpret_cast<void*>(SendNetMsg::HookFunc));

	CanLoadThirdPartyFiles::hook = new GameHook(
		Utils::GetVTable(Interfaces::fileSystem)[128],
		reinterpret_cast<void*>(CanLoadThirdPartyFiles::HookFunc));

	DoPostScreenEffects::hook = new GameHook(
		Utils::GetVTable(Memory::clientMode)[45],
		reinterpret_cast<void*>(DoPostScreenEffects::HookFunc));
}

void Hooks::Game::Unhook()
{
	delete DoPostScreenEffects::hook;
	delete CanLoadThirdPartyFiles::hook;
	delete SendNetMsg::hook;
	delete EmitSound::hook;
	delete FireEvent::hook;
	delete ViewDrawFade::hook;
	delete OverrideView::hook;
	delete FrameStageNotify::hook;
	delete CreateMove::hook;
}