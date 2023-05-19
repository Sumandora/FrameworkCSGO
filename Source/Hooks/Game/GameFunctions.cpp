#include "GameFunctions.hpp"

#include "../../Interfaces.hpp"

#include "IDASignatureScanner.hpp"

void Hooks::Game::Hook()
{
	CreateMove::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMove::HookFunc));
	FrameStageNotify::hook = new class GameHook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotify::HookFunc));
	OverrideView::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideView::HookFunc));

	// This function references its name through VPROF; Search for ViewDrawFade as string and you will find the function
	ViewDrawFade::hook = new class GameHook(SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("55 48 89 e5 41 57 49 89 D7 41 56 49 89 F6 41 55 41 54 41")), Memory::GetTextSection(xorstr_("engine_client.so"))), reinterpret_cast<void*>(ViewDrawFade::HookFunc));
}

void Hooks::Game::Unhook()
{
	delete ViewDrawFade::hook;
	delete OverrideView::hook;
	delete FrameStageNotify::hook;
	delete CreateMove::hook;
}