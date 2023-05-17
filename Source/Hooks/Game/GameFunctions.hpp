#ifndef HOOKS_GAME_GAMEFUNCTIONS
#define HOOKS_GAME_GAMEFUNCTIONS

#include "../../SDK/ClientFrameStage.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/GameClass/CViewSetup.hpp"
#include "../../SDK/Math/Matrix4x4.hpp"

#include "../Hooks.hpp"

#include "../../SDK/GameClass/IMaterial.hpp"
#include "GameHook.hpp"

namespace Hooks::Game {
	namespace CreateMove {
		inline GameHook* hook;

		inline CUserCmd lastCmd{};

		bool HookFunc(void* thisptr, float flInputSampleTime, CUserCmd* cmd);
	}

	namespace FrameStageNotify {
		inline GameHook* hook;

		inline Matrix4x4 worldToScreenMatrix{};

		void HookFunc(void* thisptr, ClientFrameStage stage);
	}

	namespace OverrideView {
		inline GameHook* hook;

		inline CViewSetup lastViewSetup{};
		inline Vector cameraPosition{};

		void HookFunc(void* thisptr, CViewSetup* pSetup);
	}

	namespace ViewDrawFade {
		inline GameHook* hook;

		void HookFunc(void* thisptr, std::byte* color, IMaterial* pFadeMaterial, bool mapFullTextureToScreen);
	}

	void Hook();
	void Unhook();
}

#endif