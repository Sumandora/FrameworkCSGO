#include "../GameFunctions.hpp"

#include "../../../Features/Misc/PartyScripts.hpp"
#include "../../../Features/Semirage/Backtrack.hpp"
#include "../../../Features/Visuals/ESP/ESP.hpp"
#include "../../../Features/Visuals/Fog.hpp"
#include "../../../Features/Visuals/MainMenu.hpp"
#include "../../../Features/Visuals/NoPunch.hpp"
#include "../../../Features/Visuals/SpectatorList.hpp"
#include "../../../Interfaces.hpp"

void Hooks::Game::FrameStageNotify::HookFunc(void* thisptr, ClientFrameStage stage)
{
	switch (stage) {
	case ClientFrameStage::FRAME_START: {
		spectatorList.Update();
		esp.Update();
		partyScripts.PerformPartyScripts();
		mainMenu.UpdateVisibility();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_START: {
		fog.FrameStageNotify();
		backtrack.FrameStageNotify();

		noPunch.HidePunch();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_END: {
		noPunch.RestorePunch();

		worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
		break;
	}
	default:
		// ignored
		break;
	}
	return InvokeFunction<void, void*, ClientFrameStage>(hook->proxy, thisptr, stage);
}
