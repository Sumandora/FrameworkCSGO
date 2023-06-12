#ifndef FEATURES_VISUALS_ESP_ENTITYCACHE
#define FEATURES_VISUALS_ESP_ENTITYCACHE

#include "../../../../SDK/GameClass/CBaseCombatWeapon.hpp"
#include "../../../../SDK/GameClass/CBaseEntity.hpp"
#include "../../../../SDK/GameClass/CBaseGrenade.hpp"
#include "../../../../SDK/GameClass/CBasePlayer.hpp"
#include "../../../../SDK/GameClass/CDrone.hpp"
#include "../../../../SDK/GameClass/CDronegun.hpp"
#include "../../../../SDK/GameClass/CHostage.hpp"
#include "../../../../SDK/GameClass/CPhysPropLootCrate.hpp"
#include "../../../../SDK/GameClass/CPlantedC4.hpp"

#include "imgui.h"

#include <unordered_map>
#include <vector>

class ScreenRectangle {
	Vector origin;
	std::optional<BoundingBox> boundingBox;

	bool CalculateScreenRectangle(ImVec4& rectangle);
	bool HandleOutOfView(const Vector& localOrigin, const Vector& viewangles, ImVec4& rectangle) const;

public:
	void Update(const Vector& origin, const std::optional<BoundingBox>& boundingBox);

	std::optional<ImVec4> Get();
};

class Entity {
public:
	CBaseEntity* backingEntity;

	int index;
	CBaseHandle handle;
	ClientClass* clientClass;

	TeamID team;
	Vector origin;

	std::optional<BoundingBox> boundingBox;

	bool spotted;

	ScreenRectangle screenRectangle;

	bool markForRemoval; // This variable tracks if the entity is marked for removal

	void Update(CBaseEntity* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);

	bool operator==(const Entity& other) const
	{
		return handle == other.handle;
	}
};

class Weapon : public Entity {
public:
	WeaponID weaponID;
	int ammo;
	int reserveAmmoCount;
	int ownerEntity;

	void Update(CBaseCombatWeapon* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class Player : public Entity {
public:
	int health;
	float flashAlpha;
	bool enemy;
	bool alive;
	bool visible;
	WeaponID activeWeapon;
	bool dormant;

	void Update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class Spectator : public Player {
public:
	ObserverMode observerMode;
	CBaseHandle observerTarget;

	void Update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class LocalPlayer : public Player {
public:
	ObserverMode observerMode;
	CBaseHandle observerTarget;
	Vector viewangles;

	LocalPlayer(CBasePlayer* entity, int index);
};

class PlantedC4 : public Entity {
public:
	bool defused;
	bool bombTicking;
	float bombTime;
	CBaseHandle defuser;
	float defuseCountDown;

	void Update(CPlantedC4* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class Hostage : public Entity {
public:
	int state;
	float grabTime;

	void Update(CHostage* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

enum class ProjectileType {
	INVALID,

	BREACH_CHARGE,
	BUMP_MINE,
	DECOY,
	MOLOTOV,
	SENSOR_GRENADE,
	SMOKE_GRENADE,
	SNOWBALL,
	HIGH_EXPLOSIVE_GRENADE,
	FLASHBANG
};

class Projectile : public Entity {
public:
	std::vector<Vector> trail;
	CBaseHandle thrower;
	ProjectileType type;

	void Update(CBaseGrenade* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

enum class LootCrateType {
	INVALID,

	PISTOL_CASE,
	LIGHT_CASE,
	HEAVY_CASE,
	EXPLOSIVE_CASE,
	TOOLS_CASE,
	CASH_DUFFLEBAG,
	RANDOM_DROP
};

class LootCrate : public Entity {
public:
	int health;
	int maxHealth;

	LootCrateType type;

	void Update(CPhysPropLootCrate* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class Drone : public Entity {
public:
	CBaseHandle target;

	void Update(CDrone* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

class Sentry : public Entity {
public:
	int health;

	void Update(CDronegun* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
};

namespace EntityCache {
	inline std::optional<LocalPlayer> localPlayer;

	inline std::unordered_map<CBaseHandle, Entity> entities;
	inline std::unordered_map<CBaseHandle, Player> players;
	inline std::unordered_map<CBaseHandle, Spectator> spectators;
	inline std::unordered_map<CBaseHandle, Weapon> weapons;
	inline std::unordered_map<CBaseHandle, Hostage> hostages;
	inline std::unordered_map<CBaseHandle, Projectile> projectiles;
	inline std::unordered_map<CBaseHandle, PlantedC4> bombs;
	inline std::unordered_map<CBaseHandle, Entity> ammoBoxes;
	inline std::unordered_map<CBaseHandle, LootCrate> lootCrates;
	inline std::unordered_map<CBaseHandle, Drone> drones;
	inline std::unordered_map<CBaseHandle, Sentry> sentries;

	Player* PlayerByHandle(const CBaseHandle& handle);

	void UpdateEntities(
		int maxDistance,
		bool captureEntities,
		bool capturePlayers,
		bool captureSpectators,
		bool captureWeapons,
		bool captureHostages,
		bool captureProjectiles,
		bool captureBombs,
		bool captureAmmoBoxes,
		bool captureLootCrates,
		bool captureDrones,
		bool captureSentries);
}

#endif
