#include "EntityCache.hpp"

#include <array>

#include "../../../../Interfaces.hpp"

// TODO Only capture them if needed
static std::optional<LocalPlayer> localPlayer;
static std::vector<Entity> entities;
static std::vector<Player> players;
static std::vector<Spectator> spectators;
static std::vector<Weapon> weapons;
static std::vector<Hostage> hostages;
static std::vector<Projectile> projectiles;
static std::vector<PlantedC4> bombs;
static std::vector<LootCrate> lootCrates;
static std::vector<Drone> drones;
static std::vector<Sentry> sentries;

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
	auto playerPtr = std::ranges::find(vector, handle, &Entity::handle);
	return playerPtr != vector.end() ? &(*playerPtr) : nullptr;
}

template <typename E = Entity, typename P = CBaseEntity*>
static void UpdateEntity(std::vector<E>& vector, P p, int index, CBaseHandle handle, ClientClass* clientClass)
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
	if (localPlayerEntity)
		localPlayer = LocalPlayer(localPlayerEntity, localPlayerIndex);

	auto MarkForRemoval = [](Entity& entity) {
		entity.markForRemoval = true;
	};
	// Mark all entities for removal, they will unmark themselves when updated
	std::for_each(entities.begin(), entities.end(), MarkForRemoval);
	std::for_each(players.begin(), players.end(), MarkForRemoval);
	std::for_each(spectators.begin(), spectators.end(), MarkForRemoval);
	std::for_each(weapons.begin(), weapons.end(), MarkForRemoval);
	std::for_each(hostages.begin(), hostages.end(), MarkForRemoval);
	std::for_each(projectiles.begin(), projectiles.end(), MarkForRemoval);
	std::for_each(bombs.begin(), bombs.end(), MarkForRemoval);
	std::for_each(lootCrates.begin(), lootCrates.end(), MarkForRemoval);
	std::for_each(drones.begin(), drones.end(), MarkForRemoval);
	std::for_each(sentries.begin(), sentries.end(), MarkForRemoval);

	for (int index = 0; index <= Interfaces::entityList->GetHighestEntityIndex(); index++) {
		if (localPlayerIndex == index)
			continue;

		CBaseEntity* entity = Interfaces::entityList->GetClientEntity(index);
		if (!entity)
			continue;

		if ((*entity->Origin() - localPlayer->origin).LengthSquared() > (float)(maxDistance * maxDistance))
			continue;

		const CBaseHandle handle = entity->GetRefEHandle();

		if (localPlayer->observerMode == ObserverMode::OBS_MODE_IN_EYE && localPlayer->observerTarget == handle)
			continue;

		ClientClass* clientClass = entity->GetClientClass();

		if (entity->IsPlayer()) {
			auto* player = reinterpret_cast<CBasePlayer*>(entity);
			const TeamID team = *player->Team();
			if (team != TeamID::TEAM_UNASSIGNED) {
				if (player->IsAlive())
					UpdateEntity<Player, CBasePlayer*>(players, player, index, handle, clientClass);
				else
					UpdateEntity<Spectator, CBasePlayer*>(spectators, player, index, handle, clientClass);
			}
			continue;
		} else {
			if (entity->GetDormant())
				continue;

			if (entity->IsWeaponWorldModel()) {
				UpdateEntity<Weapon, CBaseCombatWeapon*>(weapons, reinterpret_cast<CBaseCombatWeapon*>(entity), index,
					handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CPlantedC4) {
				UpdateEntity<PlantedC4, CPlantedC4*>(bombs, reinterpret_cast<CPlantedC4*>(entity), index, handle,
					clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CHostage) {
				UpdateEntity<Hostage, CHostage*>(hostages, reinterpret_cast<CHostage*>(entity), index, handle, clientClass);
				continue;
			} else if(clientClass->m_ClassID == ClientClassID::CPhysPropLootCrate) {
				UpdateEntity<LootCrate, CPhysPropLootCrate*>(lootCrates, reinterpret_cast<CPhysPropLootCrate*>(entity), index, handle, clientClass);
				continue;
			} else if(clientClass->m_ClassID == ClientClassID::CDrone) {
				UpdateEntity<Drone, CDrone*>(drones, reinterpret_cast<CDrone*>(entity), index, handle, clientClass);
				continue;
			} else if(clientClass->m_ClassID == ClientClassID::CDronegun) {
				UpdateEntity<Sentry, CDronegun*>(sentries, reinterpret_cast<CDronegun*>(entity), index, handle, clientClass);
				continue;
			} else if (std::find(projectileClassIDs.begin(), projectileClassIDs.end(), clientClass->m_ClassID) != projectileClassIDs.end()) {
				UpdateEntity<Projectile, CBaseEntity*>(projectiles, entity, index, handle, clientClass);
				continue;
			}
		}

		UpdateEntity<>(entities, entity, index, handle, clientClass);
	}

	auto EraseMarkedForRemoval = [](Entity& entity) { // Remove old records
		return entity.markForRemoval;
	};

	std::erase_if(entities, EraseMarkedForRemoval);
	std::erase_if(players, EraseMarkedForRemoval);
	std::erase_if(spectators, EraseMarkedForRemoval);
	std::erase_if(weapons, EraseMarkedForRemoval);
	std::erase_if(hostages, EraseMarkedForRemoval);
	std::erase_if(projectiles, EraseMarkedForRemoval);
	std::erase_if(bombs, EraseMarkedForRemoval);
	std::erase_if(lootCrates, EraseMarkedForRemoval);
	std::erase_if(drones, EraseMarkedForRemoval);
	std::erase_if(sentries, EraseMarkedForRemoval);
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

std::vector<Spectator>& EntityCache::GetSpectators()
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

std::vector<LootCrate>& EntityCache::GetLootCrates()
{
	return lootCrates;
}

std::vector<Drone>& EntityCache::GetDrones()
{
	return drones;
}

std::vector<Sentry>& EntityCache::GetSentries()
{
	return sentries;
}