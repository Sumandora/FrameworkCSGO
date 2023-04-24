#include "FrameStageNotifyHook.hpp"

#include "../../../Interfaces.hpp"

#include "../../../Features/Semirage.hpp"
#include "../../../Features/Visuals.hpp"
#include "../../../GameCache.hpp"

#include "../GameHook.hpp"

static GameHook* hook;

void FrameStageNotifyHook(void* thisptr, ClientFrameStage stage)
{
	switch (stage) {
	case ClientFrameStage::FRAME_START: {
		GameCache::ClearLocalPlayer();
		Features::Visuals::Esp::UpdateVisibility();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_START: {
		Features::Semirage::Backtrack::FrameStageNotify();

		Features::Visuals::NoPunch::HidePunch();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_END: {
		Features::Visuals::NoPunch::RestorePunch();

		Hooks::FrameStageNotify::worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
		break;
	}
	default:
		// ignored
		break;
	}
	return InvokeFunction<void, void*, ClientFrameStage>(hook->proxy, thisptr, stage);
}

void Hooks::FrameStageNotify::Hook()
{
	hook = new class GameHook(Utils::GetVTable(Interfaces::baseClient)[37], reinterpret_cast<void*>(FrameStageNotifyHook));
}

void Hooks::FrameStageNotify::Unhook()
{
	delete hook;
}
