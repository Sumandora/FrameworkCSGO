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
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("CGameEventManager::FireEvent")
			.findXREFs("engine_client.so", true, false)
			.prevByteOccurence("55 48 89 f8 48 89 e5")
			.getPointer()
			.value(),
		reinterpret_cast<void*>(FireEvent::HookFunc));

	EmitSound::hook = new GameHook(
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("EmitSound: %s pitch out of bounds = %i\n")
			.findXREFs("engine_client.so", true, false)
			.filter([](const BCRL::SafePointer& safePointer) { return safePointer.add(4).equals('\xe8'); })
			.prevByteOccurence("55 66 0f ef db")
			.getPointer()
			.value(),
		reinterpret_cast<void*>(EmitSound::HookFunc));

	SendNetMsg::hook = new GameHook(
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("SendNetMsg %s: stream[%s] buffer overflow (maxsize = %d)!\n")
			.findXREFs("engine_client.so", true, false)
			.filter([](const BCRL::SafePointer& safePointer) { return safePointer.add(4).doesMatch("44 89 e9"); })
			.prevByteOccurence("55 48 89 e5 41 57")
			.getPointer()
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