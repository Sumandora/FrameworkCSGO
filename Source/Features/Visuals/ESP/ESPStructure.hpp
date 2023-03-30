#ifndef FEATURES_LEGIT_ESPSTRUCTURE
#define FEATURES_LEGIT_ESPSTRUCTURE

#include "../../../GUI/ImGuiColors.hpp"
#include "imgui.h"

#include "../../../SDK/GameClass/CBaseCombatWeapon.hpp"
#include "../../../SDK/GameClass/CPlantedC4.hpp"

#include "../../../SDK/GameClass/CBasePlayer.hpp"
#include "../../../Serialization/Serializer.hpp"

// Base Types

// TODO Generate the boilerplate for implementations
class BoxSettings {
private:
	bool enabled;
	ImColor color;
	float rounding;
	float thickness;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;
	bool fill;
	ImColor fillColor;

public:
	BoxSettings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle) const;
	bool operator<=>(const BoxSettings& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class TextSetting {
public:
	bool enabled;

private:
	float fontScale;
	ImColor fontColor;
	bool shadow;
	ImColor shadowColor;

public:
	TextSetting();

	void Draw(ImDrawList* drawList, float x, float y, bool centered, const char* text) const;
	[[nodiscard]] float GetLineHeight() const;

	bool operator<=>(const TextSetting& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class HealthbarSettings {
private:
	bool enabled;
	ImColor backgroundColor;
	float rounding;
	float spacing;
	float width;
	ImColor aliveColor;
	ImColor deadColor;
	bool gradient;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;
	TextSetting healthNumber;
	bool onlyWhenDamaged;

public:
	HealthbarSettings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle, int health) const;
	bool operator<=>(const HealthbarSettings& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class BoxNameSetting {
public:
	explicit BoxNameSetting();

	BoxSettings box;
	TextSetting nametag;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) const;
	bool operator<=>(const BoxNameSetting& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerStateSettings {
public:
	PlayerStateSettings();

	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	TextSetting flashDuration;
	// TODO Flags

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player) const;
	bool operator<=>(const PlayerStateSettings& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class WeaponSettings {
private:
	BoxNameSetting boxName;
	TextSetting ammo;

public:
	WeaponSettings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlantedC4Settings {
private:
	BoxNameSetting boxName;
	TextSetting timer;
	bool showDefuseTimer;

public:
	PlantedC4Settings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

// Combination

class PlayerTeamSettings {
public:
	PlayerTeamSettings();

	PlayerStateSettings visible;
	PlayerStateSettings occluded;
	PlayerStateSettings dormant;

	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerSettings {
public:
	PlayerSettings();

	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;
	BoxNameSetting spectators;

	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

#endif