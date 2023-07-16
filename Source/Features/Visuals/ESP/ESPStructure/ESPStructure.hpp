#ifndef FEATURES_VISUALS_ESP_ESPSTRUCTURE
#define FEATURES_VISUALS_ESP_ESPSTRUCTURE

#include "imgui.h"

#include "../EntityCache/EntityCache.hpp"

#include "PlayerFlags.hpp"
#include <memory>

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
	[[nodiscard]] float GetLineWidth() const;

	bool operator<=>(const BoxSettings& other) const = default;
	void SetupGUI(const char* id);
	SERIALIZER();
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
	SERIALIZER();
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
	SERIALIZER();
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
	SERIALIZER();
};

class FlagsSetting {
private:
	bool enabled;
	float fontScale;
	float verticalSpacing;
	bool shadow;
	ImColor shadowColor;

	std::vector<std::shared_ptr<Flag>> flags;

public:
	FlagsSetting(std::vector<std::shared_ptr<Flag>>&& flags);

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, float x, float y, const Player& player) const;
	bool operator<=>(const FlagsSetting& other) const = default;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class BoxNameSetting {
public:
	BoxSettings box;
	TextSetting nametag;

	BoxNameSetting() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, const ImVec4& rectangle, const char* text) const;
	bool operator<=>(const BoxNameSetting& other) const = default;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class GenericEntitySettings {
public:
	BoxNameSetting boxName;

	GenericEntitySettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Entity& entity, const char* text) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class SentrySettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;

public:
	SentrySettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Sentry& sentry) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class LootCrateSettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;

public:
	LootCrateSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, LootCrate& lootCrate, const char* name) const;
	void SetupGUI(const char* id);
	SERIALIZER();
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

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, LootCrate& lootCrate) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class DroneSettings {
	BoxNameSetting boxName;
	TextSetting target;

public:
	DroneSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Drone& drone) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class PlayerStateSettings {
	BoxNameSetting boxName;
	HealthbarSettings healthbar;
	TextSetting weapon;
	FlagsSetting flags;

public:
	PlayerStateSettings();

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Player& player) const;
	bool operator<=>(const PlayerStateSettings& other) const = default;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class WeaponSettings {
	BoxNameSetting boxName;
	TextSetting ammo;

public:
	WeaponSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Weapon& weapon) const;
	void SetupGUI(const char* id);
	SERIALIZER();
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

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, PlantedC4& bomb) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class HostageSettings {
	BoxNameSetting boxName;
	TextSetting timer;
	int accuracy;

public:
	HostageSettings();

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Hostage& hostage) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class SpectatorSettings {
	BoxNameSetting boxName;
	bool onlyShowWhenSpectatedEntityIsDormant;
	bool displayNameOfSpectatedEntity;

public:
	SpectatorSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Spectator& spectator) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class ProjectileSettings {
	BoxNameSetting boxName;
	TextSetting ownerName;
	LineSetting trail;

public:
	ProjectileSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Projectile& projectile, const char* name) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class ProjectileTypeSettings {
	mutable ProjectileSettings breachCharge;
	mutable ProjectileSettings bumpMine;
	mutable ProjectileSettings decoy;
	mutable ProjectileSettings molotov;
	mutable ProjectileSettings sensorGrenade;
	mutable ProjectileSettings smokeGrenade;
	mutable ProjectileSettings snowball;
	mutable ProjectileSettings highExplosiveGrenade;
	mutable ProjectileSettings flashbang;

	ProjectileSettings& GetSettings(ProjectileType type) const;

public:
	ProjectileTypeSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Projectile& projectile) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class PlayerTeamSettings {
public:
	PlayerStateSettings visible;
	PlayerStateSettings occluded;

	PlayerTeamSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Player& player) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

class PlayerSettings {
public:
	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;

	PlayerSettings() = default;

	[[nodiscard]] bool IsEnabled() const;
	void Draw(ImDrawList* drawList, Player& player) const;
	void SetupGUI(const char* id);
	SERIALIZER();
};

#endif