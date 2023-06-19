#ifndef HOOKS_GAME_GAMEFUNCTIONS
#define HOOKS_GAME_GAMEFUNCTIONS

#include "../../SDK/ClientFrameStage.hpp"
#include "../../SDK/GameClass/CGameEvent.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/GameClass/CViewSetup.hpp"
#include "../../SDK/GameClass/IMaterial.hpp"
#include "../../SDK/Math/Matrix4x4.hpp"

#include "../Hooks.hpp"
#include "GameHook.hpp"

#include <unordered_map>
#include <vector>

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

		void HookFunc(void* thisptr, CViewSetup* pSetup);
	}

	namespace ViewDrawFade {
		inline GameHook* hook;

		void HookFunc(void* thisptr, std::byte* color, IMaterial* pFadeMaterial, bool mapFullTextureToScreen);
	}

	namespace FireEvent {
		inline GameHook* hook;

		inline std::unordered_map<std::string, int> events;

		void HookFunc(void* thisptr, CGameEvent* event, bool bServerOnly, bool bClientOnly);
	}

	void Hook();
	void Unhook();
}

#endif