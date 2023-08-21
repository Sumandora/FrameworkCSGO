#ifndef HOOKS_GAME_GAMEFUNCTIONS
#define HOOKS_GAME_GAMEFUNCTIONS

#include "../../SDK/ClientFrameStage.hpp"
#include "../../SDK/GameClass/CGameEvent.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/GameClass/CViewSetup.hpp"
#include "../../SDK/GameClass/IMaterial.hpp"
#include "../../SDK/GameClass/INetMessage.hpp"
#include "../../SDK/Math/Matrix4x4.hpp"

#include "../Hooks.hpp"
#include "GameHook.hpp"

#include <unordered_map>
#include <vector>

namespace Hooks::Game {
	namespace CreateMove {
		inline GameHook* hook;

		inline CUserCmd lastCmd{};

		bool hookFunc(void* thisptr, float flInputSampleTime, CUserCmd* cmd);
	}

	namespace FrameStageNotify {
		inline GameHook* hook;

		inline Matrix4x4 worldToScreenMatrix{};

		void hookFunc(void* thisptr, ClientFrameStage stage);
	}

	namespace OverrideView {
		inline GameHook* hook;

		inline CViewSetup lastViewSetup{};

		void hookFunc(void* thisptr, CViewSetup* pSetup);
	}

	namespace ViewDrawFade {
		inline GameHook* hook;

		void hookFunc(void* thisptr, std::byte* color, IMaterial* pFadeMaterial, bool mapFullTextureToScreen);
	}

	namespace FireEvent {
		inline GameHook* hook;

		inline std::unordered_map<std::string, int> events;

		void hookFunc(void* thisptr, CGameEvent* event, bool bServerOnly, bool bClientOnly);
	}

	namespace EmitSound {
		inline GameHook* hook;

		int hookFunc(void* thisptr,
			void* filter,
			int iEntIndex,
			int iChannel,
			const char* pSoundEntry,
			unsigned int nSoundEntryHash,
			const char* pSample,
			float flVolume,
			int iSoundLevel,
			int nSeed,
			int iFlags,
			int iPitch,
			const Vector* pOrigin,
			const Vector* pDirection,
			void* pUtlVecOrigins,
			bool bUpdatePositions,
			float soundtime,
			int speakerentity,
			void* params);
	}

	namespace SendNetMsg {
		inline GameHook* hook;

		bool hookFunc(void* thisptr, INetMessage* msg, bool bForceReliable, bool bVoice);
	}

	namespace CanLoadThirdPartyFiles {
		inline GameHook* hook;

		bool hookFunc(void* thisptr);
	}

	namespace DoPostScreenEffects {
		inline GameHook* hook;

		bool hookFunc(void* thisptr, void* rsi);
	}

	void hook();
	void unhook();
}

#endif