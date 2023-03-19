#ifndef FEATURES_LEGIT
#define FEATURES_LEGIT

#include "../SDK/CUserCmd.hpp"
#include "../Serialization/Serializer.hpp"
#include "Legit/ESP/ESPStructure.hpp"
#include "SDL.h"

namespace Features::Legit {
	namespace Aimbot {
		// Settings
		extern bool enabled;
		extern float fov;
		extern float smoothness;
		extern int maximalInfluence;
		extern int maximalFlashAmount;
		extern bool dontAimThroughSmoke;
		extern bool friendlyFire;

		void PollEvent(SDL_Event* event);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace SpectatorList {
		// Settings
		extern bool enabled;

		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Esp {
		// Settings
		extern bool enabled;
		extern int onKey;
		extern int drawDistance;
		extern bool considerSpottedEntitiesAsVisible;
		extern bool considerSmokedOffEntitiesAsOccluded;
		extern PlayerSettings players;
		extern WeaponSettings weapons;
		extern BoxNameSetting projectiles;
		extern PlantedC4Settings plantedC4;
		extern BoxNameSetting hostages;
		extern BoxNameSetting dzLootCrates;
		extern BoxNameSetting dzAmmoBoxes;
		extern BoxNameSetting dzSentries;
		extern BoxNameSetting other;

		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Triggerbot {
		// Settings
		extern bool enabled;
		extern int input;
		extern bool secondaryFireWithR8Revolver;
		extern bool friendlyFire;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
