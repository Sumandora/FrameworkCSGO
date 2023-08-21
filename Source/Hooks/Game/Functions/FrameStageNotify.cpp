#include "../GameFunctions.hpp"

#include "../../../Features/Misc/PartyScripts.hpp"
#include "../../../Features/Semirage/Backtrack.hpp"
#include "../../../Features/Visuals/ESP/ESP.hpp"
#include "../../../Features/Visuals/Fog.hpp"
#include "../../../Features/Visuals/MainMenu.hpp"
#include "../../../Features/Visuals/NoPunch.hpp"
#include "../../../Features/Visuals/SpectatorList.hpp"
#include "../../../Interfaces.hpp"

void Hooks::Game::FrameStageNotify::hookFunc(void* thisptr, ClientFrameStage stage)
{
	switch (stage) {
	case ClientFrameStage::FRAME_START: {
		spectatorList.update();
		esp.update();
		partyScripts.performPartyScripts();
		mainMenu.updateVisibility();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_START: {
		fog.frameStageNotify();
		backtrack.frameStageNotify();

		noPunch.hidePunch();
		break;
	}
	case ClientFrameStage::FRAME_RENDER_END: {
		noPunch.restorePunch();

		worldToScreenMatrix = *Interfaces::engine->WorldToScreenMatrix();
		break;
	}
	default:
		// ignored
		break;
	}
	return invokeFunction<void, void*, ClientFrameStage>(hook->proxy, thisptr, stage);
}
