#ifndef FEATURES_LEGIT_ESP
#define FEATURES_LEGIT_ESP

#include "ESPStructure.hpp"
#include "xorstr.hpp"

namespace Features::Legit::Esp {
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
	extern BoxNameSetting dzLootCrates;
	extern BoxNameSetting dzAmmoBoxes;
	extern BoxNameSetting dzSentries;

	void ImGuiRender(ImDrawList* drawList);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
