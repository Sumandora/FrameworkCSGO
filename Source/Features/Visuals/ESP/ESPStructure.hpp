#ifndef FEATURES_VISUALS_ESP_ESPSTRUCTURE
#define FEATURES_VISUALS_ESP_ESPSTRUCTURE

#include "imgui.h"

#include "../../../Serialization/Serializer.hpp"

#include "EntityCache/EntityCache.hpp"

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

	void Draw(ImDrawList* drawList, float x, float y, bool centered, const char* text, std::optional<ImColor> colorOverride = std::nullopt) const;
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

class LineSetting {
public:
	bool enabled;

private:
	ImColor lineColor;
	float thickness;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;

public:
	LineSetting();

	void Draw(ImDrawList* drawList, std::vector<ImVec2> points) const;

	bool operator<=>(const LineSetting& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class BoxNameSetting {
public:
	explicit BoxNameSetting() = default;

	BoxSettings box;
	TextSetting nametag;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) const;
	bool operator<=>(const BoxNameSetting& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerStateSettings {
public:
	PlayerStateSettings() = default;

	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	TextSetting flashDuration; // Make flag with opacity
	// TODO Flags

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const Player& player) const;
	bool operator<=>(const PlayerStateSettings& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class WeaponSettings {
private:
	BoxNameSetting boxName;
	TextSetting ammo;

public:
	WeaponSettings() = default;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const Weapon& weapon) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlantedC4Settings {
private:
	BoxNameSetting boxName;
	TextSetting timer;
	TextSetting defuseTimer;
	bool overrideDefuseColor;
	ImColor defusePossibleWithKit;
	ImColor defuseImpossible;
	int accuracy;

public:
	PlantedC4Settings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const PlantedC4& bomb) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class HostageSettings {
private:
	BoxNameSetting boxName;
	TextSetting timer;
	int accuracy;

public:
	HostageSettings();

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const Hostage& hostage) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class ProjectileSettings {
private:
	BoxNameSetting boxName;
	LineSetting trail;

public:
	ProjectileSettings() = default;

	void Draw(ImDrawList* drawList, ImVec4 rectangle, const Projectile& projectile) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerTeamSettings {
public:
	PlayerTeamSettings() = default;

	PlayerStateSettings visible;
	PlayerStateSettings occluded;

	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerSettings {
public:
	PlayerSettings() = default;

	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;
	BoxNameSetting spectators;

	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

#endif