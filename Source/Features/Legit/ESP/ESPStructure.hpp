#ifndef FEATURES_LEGIT_ESPSTRUCTURE
#define FEATURES_LEGIT_ESPSTRUCTURE

#include "imgui.h"
#include "../../../GUI/ImGuiColors.hpp"

#include "../../../SDK/GameClass/CBaseCombatWeapon.hpp"
#include "../../../SDK/GameClass/CPlantedC4.hpp"

class BoxSettings {
public:
	bool	enabled = false;
	ImColor	color = ImGuiColors::white;
	float	rounding = 0.0f;
	float	thickness = 1.0f;
	bool	outlined = false;
	ImColor	outlineColor = ImGuiColors::black;
	float	outlineThickness = 1.0f;
	bool	fill = false;
	ImColor	fillColor = ImGuiColors::white;

	void Draw(ImDrawList* drawList, ImVec4 rectangle);
	void SetupGUI(const char* id);
};

class HealthbarSettings {
public:
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

	void Draw(ImDrawList* drawList, ImVec4 rectangle, float health);
	void SetupGUI(const char* id);
};

class TextSetting {
public:
	bool	enabled = false;
	float	fontScale = 1.0f;
	ImColor	fontColor = ImGuiColors::white;
	bool	shadow = false;
	ImColor	shadowColor = ImGuiColors::black;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text, float height);
	void SetupGUI(const char* id);
};

class BoxNameSetting {
public:
	BoxSettings	box{};
	TextSetting	nametag{};

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text);
	void SetupGUI(const char* id);
};

class PlayerStateSettings {
public:
	BoxNameSetting	  boxName{};
	HealthbarSettings	healthbar{};
	TextSetting			weapon{};
	TextSetting			flashDuration{};

	void SetupGUI(const char* id);
};

class WeaponSettings {
public:
	BoxNameSetting  boxName{};
	TextSetting	 ammo{};

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon);
	void SetupGUI(const char* id);
};

class PlantedC4Settings {
public:
	BoxNameSetting  boxName{};
	TextSetting	 timer{};

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb);
	void SetupGUI(const char* id);
};

#endif