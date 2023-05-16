#include "Hooks/Game/GameFunctions.hpp"

#include "GameCache.hpp"
#include "Interfaces.hpp"
#include "Features/Semirage/Semirage.hpp"
#include "Features/Visuals/Visuals.hpp"

void Hooks::Game::FrameStageNotify::HookFunc(void* thisptr, ClientFrameStage stage)
{
	switch (stage) {
	case ClientFrameStage::FRAME_START: {
		GameCache::ClearLocalPlayer();
		Features::Visuals::SpectatorList::Update();
		Features::Visuals::Esp::Update();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_START: {
		Features::Semirage::Backtrack::FrameStageNotify();

		Features::Visuals::NoPunch::HidePunch();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_END: {
		Features::Visuals::NoPunch::RestorePunch();

		worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
		break;
	}
	default:
		// ignored
		break;
	}
	return InvokeFunction<void, void*, ClientFrameStage>(hook->proxy, thisptr, stage);
}
