#include "EntityCache.hpp"

#include <array>

#include "../../../../Interfaces.hpp"

static std::optional<LocalPlayer> localPlayer;
static std::vector<Entity> entities;
static std::vector<Player> players;
static std::vector<Player> spectators;
static std::vector<Weapon> weapons;
static std::vector<Hostage> hostages;
static std::vector<Projectile> projectiles;
static std::vector<PlantedC4> bombs;

std::array<ClientClassID, 8> projectileClassIDs = {
	ClientClassID::CBaseCSGrenadeProjectile,
	ClientClassID::CBreachChargeProjectile,
	ClientClassID::CBumpMineProjectile,
	ClientClassID::CDecoyProjectile,
	ClientClassID::CMolotovProjectile,
	ClientClassID::CSensorGrenadeProjectile,
	ClientClassID::CSmokeGrenadeProjectile,
	ClientClassID::CSnowballProjectile
};

template <typename E = Entity>
inline E* EntityByHandle(std::vector<E>& vector, CBaseHandle handle)
{
	for (E& e : vector) {
		if (e.handle == handle)
			return &e;
	}
	return nullptr;
}

template <typename E = Entity, typename P = CBaseEntity*>
E* UpdateEntity(std::vector<E>& vector, P p, int index, CBaseHandle handle, ClientClass* clientClass)
{
	auto* playerPtr = EntityByHandle<E>(vector, handle);
	if (playerPtr)
		playerPtr->Update(p, index, handle, clientClass);
	else {
		E e;
		e.Update(p, index, handle, clientClass);
		vector.push_back(e);
	}
}

void EntityCache::UpdateEntities(int maxDistance)
{
	static int lastTickcount;
	if (lastTickcount == Memory::globalVars->tickcount)
		return;
	lastTickcount = Memory::globalVars->tickcount;

	static std::mutex mutex;
	const std::lock_guard<std::mutex> lock(mutex); // Since the game is multi-threaded we want to have valid data here 24/7

	localPlayer.reset();

	const int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	auto* localPlayerEntity = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if(localPlayerEntity)
		localPlayer = LocalPlayer(localPlayerEntity, localPlayerIndex);

	std::vector<CBaseEntity*> allEntities;

	for (int index = 0; index <= Interfaces::entityList->GetHighestEntityIndex(); index++) {
		if (localPlayerIndex == index)
			continue;

		CBaseEntity* entity = Interfaces::entityList->GetClientEntity(index);
		if (!entity)
			continue;

		if ((*entity->Origin() - localPlayer->origin).LengthSquared() > (float)(maxDistance * maxDistance))
			continue;

		if (entity->GetDormant())
			continue;

		const CBaseHandle handle = entity->GetRefEHandle();

		if(localPlayer->observerMode == ObserverMode::OBS_MODE_IN_EYE && localPlayer->observerTarget == handle)
			continue;

		allEntities.push_back(entity);

		ClientClass* clientClass = entity->GetClientClass();

		if (entity->IsPlayer()) {
			auto* player = reinterpret_cast<CBasePlayer*>(entity);
			const TeamID team = *player->Team();
			if (team != TeamID::TEAM_UNASSIGNED) {
				if (player->IsAlive())
					UpdateEntity<Player, CBasePlayer*>(players, player, index, handle, clientClass);
				else
					UpdateEntity<Player, CBasePlayer*>(spectators, player, index, handle, clientClass);
			}
			continue;
		} else if(entity->IsWeaponWorldModel()) {
			UpdateEntity<Weapon, CBaseCombatWeapon*>(weapons, reinterpret_cast<CBaseCombatWeapon*>(entity), index, handle, clientClass);
			continue;
		} else if(clientClass->m_ClassID == ClientClassID::CPlantedC4) {
			UpdateEntity<PlantedC4, CPlantedC4*>(bombs, reinterpret_cast<CPlantedC4*>(entity), index, handle, clientClass);
			continue;
		} else if(clientClass->m_ClassID == ClientClassID::CHostage) {
			UpdateEntity<Hostage, CHostage*>(hostages, reinterpret_cast<CHostage*>(entity), index, handle, clientClass);
			continue;
		} else if(std::find(projectileClassIDs.begin(), projectileClassIDs.end(), clientClass->m_ClassID) != projectileClassIDs.end()) {
			UpdateEntity<Projectile, CBaseEntity*>(projectiles, entity, index, handle, clientClass);
			continue;
		}

		UpdateEntity<>(entities, entity, index, handle, clientClass);
	}

	auto IsInvalid = [allEntities](const Entity& entity) { // Remove old records
		return std::find(allEntities.begin(), allEntities.end(), entity.backingEntity) == allEntities.end();
	};

	std::erase_if(entities, IsInvalid);
	std::erase_if(players, IsInvalid);
	std::erase_if(spectators, IsInvalid);
	std::erase_if(weapons, IsInvalid);
	std::erase_if(hostages, IsInvalid);
	std::erase_if(projectiles, IsInvalid);
	std::erase_if(bombs, IsInvalid);

	std::erase_if(players, [](const Player& player) { // Remove dead players
		return !reinterpret_cast<CBasePlayer*>(player.backingEntity)->IsAlive();
	});

	std::erase_if(spectators, [](const Player& player) { // Remove alive spectators
		return reinterpret_cast<CBasePlayer*>(player.backingEntity)->IsAlive();
	});
}

std::optional<LocalPlayer>& EntityCache::GetLocalPlayer()
{
	return localPlayer;
}

std::vector<Entity>& EntityCache::GetEntities()
{
	return entities;
}

std::vector<Player>& EntityCache::GetPlayers()
{
	return players;
}

std::vector<Player>& EntityCache::GetSpectators()
{
	return spectators;
}

std::vector<Weapon>& EntityCache::GetWeapons()
{
	return weapons;
}

std::vector<Hostage>& EntityCache::GetHostages()
{
	return hostages;
}

std::vector<Projectile>& EntityCache::GetProjectiles()
{
	return projectiles;
}

std::vector<PlantedC4>& EntityCache::GetBombs()
{
	return bombs;
}