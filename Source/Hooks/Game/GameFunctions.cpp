#include "GameFunctions.hpp"

#include "../../Interfaces.hpp"

void Hooks::Game::Hook() {
	CreateMove::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[25], reinterpret_cast<void*>(CreateMove::HookFunc));
	FrameStageNotify::hook = new class GameHook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotify::HookFunc));
	OverrideView::hook = new class GameHook(Utils::GetVTable(Memory::clientMode)[19], reinterpret_cast<void*>(OverrideView::HookFunc));
}

void Hooks::Game::Unhook() {
	delete CreateMove::hook;
	delete FrameStageNotify::hook;
	delete OverrideView::hook;
}