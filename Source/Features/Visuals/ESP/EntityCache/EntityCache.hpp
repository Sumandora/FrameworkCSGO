#ifndef FEATURES_VISUALS_ESP_ENTITYCACHE
#define FEATURES_VISUALS_ESP_ENTITYCACHE

#include "SDK/GameClass/CBaseCombatWeapon.hpp"
#include "SDK/GameClass/CBaseEntity.hpp"
#include "SDK/GameClass/CBasePlayer.hpp"
#include "SDK/GameClass/CPlantedC4.hpp"
#include "SDK/GameClass/CHostage.hpp"

#include "imgui.h"

#include <vector>

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

	Model* model;

	int lastScreenRectangleUpdate;
	std::optional<ImVec4> lastScreenRectangle;

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

class Projectile : public Entity {
public:
	std::vector<Vector> trail;

	void Update(CBaseEntity* entity, int index, const CBaseHandle& handle, ClientClass* clientClass);
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

	void UpdateEntities(int maxDistance);
}

#endif
