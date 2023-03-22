#ifndef FEATURES_VISUALS
#define FEATURES_VISUALS

#include "../SDK/GameClass/CViewSetup.hpp"
#include "../Serialization/Serializer.hpp"
#include "Visuals/ESP/ESPStructure.hpp"

namespace Features::Visuals {
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
	namespace SpectatorList {
		// Settings
		extern bool enabled;

		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace NoPunch {
		// Settings
		extern bool hideViewPunch;
		extern bool hideAimPunch;

		void HidePunch();
		void RestorePunch();

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace FOV {
		// Settings
		extern bool forceFOV;
		extern bool ignoreScoped;
		extern float fov;
		extern bool forceViewModel;
		extern float viewModelFovOffset;
		extern bool viewOffset;
		extern float offsetX;
		extern float offsetY;
		extern float offsetZ;
		extern float rotationOffsetX;
		extern float rotationOffsetY;
		extern float rotationOffsetZ;

		void OverrideView(CViewSetup* pSetup);

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
