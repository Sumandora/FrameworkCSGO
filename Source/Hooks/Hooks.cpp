#include "Hooks.hpp"

#include "GameFunctions/CreateMove//CreateMoveHook.hpp"
#include "GameFunctions/FrameStageNotify/FrameStageNotifyHook.hpp"
#include "GameFunctions/OverrideView/OverrideViewHook.hpp"

#include "SDL/SDLHook.hpp"

void Hooks::InstallHooks()
{
	FrameStageNotify::Hook();
	CreateMove::Hook();
	OverrideView::Hook();
	SDL::Hook();
}

void Hooks::UninstallHooks()
{
	SDL::Unhook();
	OverrideView::Unhook();
	CreateMove::Unhook();
	FrameStageNotify::Unhook();
}
