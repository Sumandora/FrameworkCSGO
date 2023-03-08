#ifndef FEATURES_LEGIT_ESP
#define FEATURES_LEGIT_ESP

#include "ESPStructure.hpp"

struct PlayerTeamSettings {
	PlayerStateSettings visible {};
	PlayerStateSettings occluded {};
	PlayerStateSettings dormant {};
};

struct PlayerSettings {
	PlayerTeamSettings teammate {};
	PlayerTeamSettings enemy {};
	PlayerStateSettings local {};
	BoxNameSetting spectators {};
};

namespace Features::Legit::Esp {
	// Settings
	extern bool enabled;
	extern int onKey;
	extern int drawDistance;
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
}

#endif
