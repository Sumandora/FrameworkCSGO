#ifndef FEATURES_LEGIT_ESP
#define FEATURES_LEGIT_ESP

#include "imgui.h"
#include "../../../GUI/ImGuiColors.hpp"

struct BoxSettings {
	bool	enabled = false;
	ImColor	color = ImGuiColors::white;
	float	rounding = 0.0f;
	float	thickness = 1.0f;
	bool	outlined = false;
	ImColor	outlineColor = ImGuiColors::black;
	float	outlineThickness = 1.0f;
	bool	fill = false;
	ImColor	fillColor = ImGuiColors::white;
};

struct HealthbarSettings {
	bool	enabled = false;
	ImColor	backgroundColor = ImGuiColors::black;
	float	rounding = 0.0f;
	float	spacing = 3.0f;
	float	width = 5.0f;
	ImColor	aliveColor = ImGuiColors::green;
	ImColor	deadColor = ImGuiColors::red;
	bool	outlined = false;
	ImColor	outlineColor = ImGuiColors::black;
	float	outlineThickness = 1.0f;
};

struct TextSetting {
	bool	enabled = false;
	float	fontScale = 1.0f;
	ImColor	fontColor = ImGuiColors::white;
	bool	shadow = false;
	ImColor	shadowColor = ImGuiColors::black;
};

struct BoxNameSetting {
	BoxSettings	box{};
	TextSetting	nametag{};
};

struct PlayerStateSettings : BoxNameSetting {
	HealthbarSettings	healthbar{};
	TextSetting			weapon{};
	TextSetting			flashDuration{};
};

struct PlayerTeamSettings {
	PlayerStateSettings	visible{};
	PlayerStateSettings	occluded{};
	PlayerStateSettings	dormant{};
};

struct PlayerSettings {
	PlayerTeamSettings	teammate{};
	PlayerTeamSettings	enemy{};
	PlayerStateSettings	local{};
	BoxNameSetting		spectators{};
};

struct WeaponSettings : BoxNameSetting {
	TextSetting	ammo{};
};

struct PlantedC4Settings : BoxNameSetting {
	TextSetting	timer{};
};

namespace Features::Legit::Esp {
	// Settings
	extern bool					enabled;
	extern int					onKey;
	extern int					drawDistance;
	extern PlayerSettings		players;
	extern WeaponSettings		weapons;
	extern BoxNameSetting		projectiles;
	extern PlantedC4Settings	plantedC4;
	extern BoxNameSetting		dzLootCrates;
	extern BoxNameSetting		dzAmmoBoxes;
	extern BoxNameSetting		dzSentries;

	void ImGuiRender(ImDrawList* drawList);

	// GUI
	void SetupGUI();
}

#endif
