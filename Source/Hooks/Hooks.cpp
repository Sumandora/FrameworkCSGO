#include "Hooks.hpp"

#include "CreateMove/CreateMoveHook.hpp"
#include "SDL/SDLHook.hpp"
#include "FrameStageNotify/FrameStageNotifyHook.hpp"

void Hooks::InstallHooks() {
	FrameStageNotify::Hook();
	CreateMove::Hook();
	SDL::Hook();
}

void Hooks::UninstallHooks() {
	SDL::Unhook();
	CreateMove::Unhook();
	FrameStageNotify::Unhook();
}