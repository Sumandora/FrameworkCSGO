#ifndef FEATURES_LEGIT_ESPSTRUCTURE
#define FEATURES_LEGIT_ESPSTRUCTURE

#include "../../../GUI/ImGuiColors.hpp"
#include "imgui.h"

#include "../../../SDK/GameClass/CBaseCombatWeapon.hpp"
#include "../../../SDK/GameClass/CPlantedC4.hpp"

#include "../../../Serialization/Serializer.hpp"

// Base Types

class BoxSettings {
private:
	const char* id;

public:
	BoxSettings(const char* id);

	bool enabled;
	ImColor color;
	float rounding;
	float thickness;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;
	bool fill;
	ImColor fillColor;

	void Draw(ImDrawList* drawList, ImVec4 rectangle);
	void Copy(BoxSettings& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class HealthbarSettings {
private:
	const char* id;

public:
	HealthbarSettings(const char* id);

	bool enabled;
	ImColor backgroundColor;
	float rounding;
	float spacing;
	float width;
	ImColor aliveColor;
	ImColor deadColor;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, float health);
	void Copy(HealthbarSettings& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class TextSetting {
private:
	const char* id;

public:
	TextSetting(const char* id);

	bool enabled;
	float fontScale;
	ImColor fontColor;
	bool shadow;
	ImColor shadowColor;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text, float height);
	void Copy(TextSetting& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class BoxNameSetting {
private:
	const char* id;

public:
	BoxNameSetting(const char* id);

	BoxSettings box;
	TextSetting nametag;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text);
	void Copy(BoxNameSetting& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class PlayerStateSettings {
private:
	const char* id;

public:
	PlayerStateSettings(const char* id);

	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	TextSetting flashDuration;

	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class WeaponSettings {
private:
	const char* id;

public:
	WeaponSettings(const char* id);

	BoxNameSetting boxName;
	TextSetting ammo;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class PlantedC4Settings {
private:
	const char* id;

public:
	PlantedC4Settings(const char* id);

	BoxNameSetting boxName;
	TextSetting timer;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

// Combination

class PlayerTeamSettings {
private:
	const char* id;

public:
	PlayerTeamSettings(const char* id);

	PlayerStateSettings visible;
	PlayerStateSettings occluded;
	PlayerStateSettings dormant;

	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class PlayerSettings {
private:
	const char* id;

public:
	PlayerSettings(const char* id);

	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;
	BoxNameSetting spectators;

	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

#endif