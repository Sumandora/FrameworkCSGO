#ifndef FEATURES_VISUALS_ESP_ENTITYCACHE
#define FEATURES_VISUALS_ESP_ENTITYCACHE

#include "../../../../SDK/GameClass/CBaseCombatWeapon.hpp"
#include "../../../../SDK/GameClass/CBaseEntity.hpp"
#include "../../../../SDK/GameClass/CBasePlayer.hpp"
#include "../../../../SDK/GameClass/CHostage.hpp"
#include "../../../../SDK/GameClass/CPlantedC4.hpp"
#include "../../../../SDK/GameClass/CDrone.hpp"
#include "../../../../SDK/GameClass/CPhysPropLootCrate.hpp"
#include "../../../../SDK/GameClass/CDronegun.hpp"

#include "imgui.h"

#include <vector>

class ScreenRectangle {
	Vector origin;
	std::optional<BoundingBox> boundingBox;

	int lastScreenRectangleUpdate;
	std::optional<ImVec4> lastScreenRectangle;

	bool CalculateScreenRectangle(ImVec4& rectangle);
	bool HandleOutOfView(const Vector& localOrigin, const Vector& viewangles, ImVec4& rectangle) const;

public:
	void Update(Vector origin, std::optional<BoundingBox> boundingBox);

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

	ProjectileType type;

	void Update(CBaseEntity* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
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
	std::optional<LocalPlayer>& GetLocalPlayer();

	std::vector<Entity>& GetEntities();
	std::vector<Player>& GetPlayers();
	std::vector<Spectator>& GetSpectators();
	std::vector<Weapon>& GetWeapons();
	std::vector<Hostage>& GetHostages();
	std::vector<Projectile>& GetProjectiles();
	std::vector<PlantedC4>& GetBombs();
	std::vector<LootCrate>& GetLootCrates();
	std::vector<Drone>& GetDrones();
	std::vector<Sentry>& GetSentries();

	void UpdateEntities(int maxDistance);
}

#endif
