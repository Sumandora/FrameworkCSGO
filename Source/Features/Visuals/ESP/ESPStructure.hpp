#ifndef FEATURES_VISUALS_ESP_ESPSTRUCTURE
#define FEATURES_VISUALS_ESP_ESPSTRUCTURE

#include "imgui.h"

#include "../../../Serialization/Serializer.hpp"

#include "EntityCache/EntityCache.hpp"

// TODO Generate the boilerplate for implementations
class BoxSettings {
public:
	bool enabled;

private:
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

	void Draw(ImDrawList* drawList, const ImVec4& rectangle) const;
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
public:
	bool enabled;

private:
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

	void Draw(ImDrawList* drawList, const ImVec4& rectangle, int health, int maxHealth = 100) const;
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
	BoxSettings box;
	TextSetting nametag;

	BoxNameSetting() = default;

	void Draw(ImDrawList* drawList, const ImVec4& rectangle, const char* text) const;
	bool operator<=>(const BoxNameSetting& other) const = default;
	[[nodiscard]] bool IsEnabled() const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class GenericEntitySettings {
public:
	BoxNameSetting boxName;

	GenericEntitySettings() = default;

	void Draw(ImDrawList* drawList, Entity& entity, const char* text) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class SentrySettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;

public:
	void Draw(ImDrawList* drawList, Sentry& sentry) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class LootCrateSettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;

public:
	void Draw(ImDrawList* drawList, LootCrate& lootCrate, const char* name) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class LootCrateTypeSettings {
	mutable LootCrateSettings pistolCase;
	mutable LootCrateSettings lightCase;
	mutable LootCrateSettings heavyCase;
	mutable LootCrateSettings explosiveCase;
	mutable LootCrateSettings toolsCase;
	mutable LootCrateSettings cashDufflebag;
	mutable LootCrateSettings randomDrop;

	LootCrateSettings& GetSettings(LootCrateType type) const;

public:
	LootCrateTypeSettings() = default;

	void Draw(ImDrawList* drawList, LootCrate& lootCrate) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class DroneSettings {
	BoxNameSetting boxName;
	TextSetting target;

public:
	DroneSettings() = default;

	void Draw(ImDrawList* drawList, Drone& drone) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlayerStateSettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	TextSetting flashDuration; // Make flag with opacity
	// TODO Flags

public:
	PlayerStateSettings() = default;

	void Draw(ImDrawList* drawList, Player& player) const;
	bool operator<=>(const PlayerStateSettings& other) const = default;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class WeaponSettings {
	BoxNameSetting boxName;
	TextSetting ammo;

public:
	WeaponSettings() = default;

	void Draw(ImDrawList* drawList, Weapon& weapon) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class PlantedC4Settings {
	BoxNameSetting boxName;
	TextSetting timer;
	TextSetting defuseTimer;
	bool overrideDefuseColor;
	ImColor defusePossibleWithKit;
	ImColor defuseImpossible;
	int accuracy;

public:
	PlantedC4Settings();

	void Draw(ImDrawList* drawList, PlantedC4& bomb) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class HostageSettings {
	BoxNameSetting boxName;
	TextSetting timer;
	int accuracy;

public:
	HostageSettings();

	void Draw(ImDrawList* drawList, Hostage& hostage) const;
	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

class ProjectileSettings {
	BoxNameSetting boxName;
	LineSetting trail; // TODO Separate settings for each type

public:
	ProjectileSettings() = default;

	void Draw(ImDrawList* drawList, Projectile& projectile) const;
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
	GenericEntitySettings spectators;

	void SetupGUI(const char* id);
	DECLARE_SERIALIZER(Serializer)
};

#endif