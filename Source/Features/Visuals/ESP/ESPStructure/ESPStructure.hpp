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

	void draw(ImDrawList* drawList, const ImVec4& rectangle) const;
	[[nodiscard]] float getLineWidth() const;

	bool operator<=>(const BoxSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class TextSettings {
public:
	bool enabled;

private:
	float fontScale;
	ImColor fontColor;
	bool shadow;
	ImColor shadowColor;

public:
	TextSettings();

	void draw(ImDrawList* drawList, float x, float y, bool centered, const char* text, std::optional<ImColor> colorOverride = std::nullopt) const;
	[[nodiscard]] float getLineHeight() const;

	bool operator<=>(const TextSettings& other) const = default;
	void setupGUI(const char* id);
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
	TextSettings healthNumber;
	bool onlyWhenDamaged;

public:
	HealthbarSettings();

	void draw(ImDrawList* drawList, const ImVec4& rectangle, int health, int maxHealth = 100) const;
	bool operator<=>(const HealthbarSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class LineSettings {
public:
	bool enabled;

private:
	ImColor lineColor;
	float thickness;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;

public:
	LineSettings();

	void draw(ImDrawList* drawList, std::vector<ImVec2> points) const;

	bool operator<=>(const LineSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class CircleSettings {
public:
	bool enabled;

private:
	ImColor circleColor;
	float radius;
	bool outlined;
	ImColor outlineColor;
	float outlineThickness;

public:
	CircleSettings();

	void draw(ImDrawList* drawList, ImVec2 center) const;

	bool operator<=>(const CircleSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class FlagsSettings {
private:
	bool enabled;
	float fontScale;
	float verticalSpacing;
	bool shadow;
	ImColor shadowColor;

	std::vector<std::shared_ptr<Flag>> flags;

public:
	FlagsSettings(std::vector<std::shared_ptr<Flag>>&& flags);

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, float x, float y, const Player& player) const;
	bool operator<=>(const FlagsSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class BoxNameSettings {
public:
	BoxSettings box;
	TextSettings nametag;

	BoxNameSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, const ImVec4& rectangle, const char* text) const;
	bool operator<=>(const BoxNameSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class GenericEntitySettings {
public:
	BoxNameSettings boxName;

	GenericEntitySettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Entity& entity, const char* text) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class SentrySettings {
	BoxNameSettings boxName;
	HealthbarSettings healthbar;

public:
	SentrySettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Sentry& sentry) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class LootCrateSettings {
	BoxNameSettings boxName;
	HealthbarSettings healthbar;

public:
	LootCrateSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, LootCrate& lootCrate, const char* name) const;
	void setupGUI(const char* id);
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

	LootCrateSettings& getSettings(LootCrateType type) const;

public:
	LootCrateTypeSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, LootCrate& lootCrate) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class DroneSettings {
	BoxNameSettings boxName;
	TextSettings target;

public:
	DroneSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Drone& drone) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class PlayerStateSettings {
	BoxNameSettings boxName;
	HealthbarSettings healthbar;
	TextSettings weapon;
	FlagsSettings flags;
	CircleSettings headDot;
	Vector headDotOffset;

public:
	PlayerStateSettings();

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Player& player) const;
	bool operator<=>(const PlayerStateSettings& other) const = default;
	void setupGUI(const char* id);
	SERIALIZER();
};

class WeaponSettings {
	BoxNameSettings boxName;
	TextSettings ammo;

public:
	WeaponSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Weapon& weapon) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class PlantedC4Settings {
	BoxNameSettings boxName;
	TextSettings timer;
	TextSettings defuseTimer;
	bool overrideDefuseColor;
	ImColor defusePossibleWithKit;
	ImColor defuseImpossible;
	int accuracy;

public:
	PlantedC4Settings();

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, PlantedC4& bomb) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class HostageSettings {
	BoxNameSettings boxName;
	TextSettings timer;
	int accuracy;

public:
	HostageSettings();

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Hostage& hostage) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class SpectatorSettings {
	BoxNameSettings boxName;
	bool onlyShowWhenSpectatedEntityIsDormant;
	bool displayNameOfSpectatedEntity;

public:
	SpectatorSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Spectator& spectator) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class ProjectileSettings {
	BoxNameSettings boxName;
	TextSettings ownerName;
	LineSettings trail;

public:
	ProjectileSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Projectile& projectile, const char* name) const;
	void setupGUI(const char* id);
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

	ProjectileSettings& getSettings(ProjectileType type) const;

public:
	ProjectileTypeSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Projectile& projectile) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class PlayerTeamSettings {
public:
	PlayerStateSettings visible;
	PlayerStateSettings occluded;

	PlayerTeamSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Player& player) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

class PlayerSettings {
public:
	PlayerTeamSettings teammate;
	PlayerTeamSettings enemy;
	PlayerStateSettings local;

	PlayerSettings() = default;

	[[nodiscard]] bool isEnabled() const;
	void draw(ImDrawList* drawList, Player& player) const;
	void setupGUI(const char* id);
	SERIALIZER();
};

#endif