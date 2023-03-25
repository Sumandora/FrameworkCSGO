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
	const char* id;

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
	explicit BoxSettings(const char* id);

	void Draw(ImDrawList* drawList, ImVec4 rectangle) const;
	bool operator==(const BoxSettings& other) const;
	void Copy(BoxSettings& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class TextSetting {
private:
	const char* id;

public:
	explicit TextSetting(const char* id);

	bool enabled;
	float fontScale;
	ImColor fontColor;
	bool shadow;
	ImColor shadowColor;

	void Draw(ImDrawList* drawList, float x, float y, bool centered, const char* text) const;
	float GetLineHeight() const;

	bool operator==(const TextSetting& other) const;
	void Copy(TextSetting& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class HealthbarSettings {
private:
	const char* id;

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
	explicit HealthbarSettings(const char* id);

	void Draw(ImDrawList* drawList, ImVec4 rectangle, int health) const;
	bool operator==(const HealthbarSettings& other) const;
	void Copy(HealthbarSettings& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class BoxNameSetting {
private:
	const char* id;

public:
	explicit BoxNameSetting(const char* id);

	BoxSettings box;
	TextSetting nametag;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) const;
	bool operator==(const BoxNameSetting& other) const;
	void Copy(BoxNameSetting& src);
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class PlayerStateSettings {
private:
	const char* id;

public:
	explicit PlayerStateSettings(const char* id);

	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	TextSetting flashDuration;
	// TODO Flags

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player) const;
	bool operator==(const PlayerStateSettings& other) const;
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class WeaponSettings {
private:
	const char* id;

	BoxNameSetting boxName;
	TextSetting ammo;

public:
	explicit WeaponSettings(const char* id);

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon) const;
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

class PlantedC4Settings {
private:
	const char* id;

	BoxNameSetting boxName;
	TextSetting timer;
	bool showDefuseTimer;

public:
	explicit PlantedC4Settings(const char* id);

	void Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb) const;
	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

// Combination

class PlayerTeamSettings {
private:
	const char* id;

public:
	explicit PlayerTeamSettings(const char* id);

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
	explicit PlayerSettings(const char* id);

	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;
	BoxNameSetting spectators;

	void SetupGUI();
	DECLARE_SERIALIZER(Serializer)
};

#endif