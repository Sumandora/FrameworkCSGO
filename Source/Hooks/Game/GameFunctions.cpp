#include "GameFunctions.hpp"

#include "../../Interfaces.hpp"

#include "BCRL.hpp"
#include "GameHook.hpp"

void Hooks::Game::hook()
{
	CreateMove::hook = new GameHook(Utils::getVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMove::hookFunc));
	FrameStageNotify::hook = new GameHook(Utils::getVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotify::hookFunc));
	OverrideView::hook = new GameHook(Utils::getVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideView::hookFunc));

	// This function references its name through VPROF; Search for ViewDrawFade as string and you will find the function
	ViewDrawFade::hook = new GameHook(Utils::getVTable(Interfaces::engineRenderView)[29], reinterpret_cast<void*>(ViewDrawFade::hookFunc));

	FireEvent::hook = new GameHook(
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("CGameEventManager::FireEvent")
			.findXREFs("engine_client.so", true, false)
			.prevByteOccurence("55 48 89 f8 48 89 e5")
			.getPointer()
			.value(),
		reinterpret_cast<void*>(FireEvent::hookFunc));

	EmitSound::hook = new GameHook(
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("EmitSound: %s pitch out of bounds = %i\n")
			.findXREFs("engine_client.so", true, false)
			.filter([](const BCRL::SafePointer& safePointer) { return safePointer.add(4).equals('\xe8'); })
			.prevByteOccurence("55 66 0f ef db")
			.getPointer()
			.value(),
		reinterpret_cast<void*>(EmitSound::hookFunc));

	SendNetMsg::hook = new GameHook(
		BCRL::Session::module("engine_client.so")
			.nextStringOccurence("SendNetMsg %s: stream[%s] buffer overflow (maxsize = %d)!\n")
			.findXREFs("engine_client.so", true, false)
			.filter([](const BCRL::SafePointer& safePointer) { return safePointer.add(4).doesMatch("44 89 e9"); })
			.prevByteOccurence("55 48 89 e5 41 57")
			.getPointer()
			.value(),
		reinterpret_cast<void*>(SendNetMsg::hookFunc));

	CanLoadThirdPartyFiles::hook = new GameHook(
		Utils::getVTable(Interfaces::fileSystem)[128],
		reinterpret_cast<void*>(CanLoadThirdPartyFiles::hookFunc));

	DoPostScreenEffects::hook = new GameHook(
		Utils::getVTable(Memory::clientMode)[45],
		reinterpret_cast<void*>(DoPostScreenEffects::hookFunc));
}

void Hooks::Game::unhook()
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