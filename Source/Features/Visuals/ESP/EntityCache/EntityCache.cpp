#include "EntityCache.hpp"

#include <array>
#include <mutex>
#include <unordered_map>

#include "../../../../Interfaces.hpp"

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

template <typename E = Entity, typename P = CBaseEntity*>
static void UpdateEntity(std::unordered_map<CBaseHandle, E>& map, P p, int index, CBaseHandle handle, ClientClass* clientClass)
{
	if (auto it = map.find(handle); it != map.end())
		it->second.Update(p, index, handle, clientClass);
	else {
		E e{};
		e.Update(p, index, handle, clientClass);
		map[handle] = e;
	}
}

Player* EntityCache::PlayerByHandle(const CBaseHandle& handle)
{
	if (handle == invalidEHandleIndex)
		return nullptr;

	if (localPlayer.has_value() && localPlayer->handle == handle)
		return &localPlayer.value();

	auto player = players.find(handle);
	return player != players.end() ? &player->second : nullptr;
}

void EntityCache::UpdateEntities(
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
	bool captureSentries)
{
	static int lastTickcount;
	if (lastTickcount == Memory::globalVars->tickcount)
		return;
	lastTickcount = Memory::globalVars->tickcount;

	static std::mutex mutex;
	const std::lock_guard<std::mutex> lock(mutex); // Since the game is multi-threaded we want to have valid data here 24/7

	localPlayer.reset();

	const int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	auto* localPlayerEntity = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if (localPlayerEntity)
		localPlayer = LocalPlayer(localPlayerEntity, localPlayerIndex);

		// Mark all entities for removal, they will unmark themselves when updated
#define MARK_FOR_REMOVAL(map)            \
	for (auto& [handle, entity] : map) { \
		entity.markForRemoval = true;    \
	}

	MARK_FOR_REMOVAL(entities)
	MARK_FOR_REMOVAL(players)
	MARK_FOR_REMOVAL(spectators)
	MARK_FOR_REMOVAL(weapons)
	MARK_FOR_REMOVAL(hostages)
	MARK_FOR_REMOVAL(projectiles)
	MARK_FOR_REMOVAL(bombs)
	MARK_FOR_REMOVAL(ammoBoxes)
	MARK_FOR_REMOVAL(lootCrates)
	MARK_FOR_REMOVAL(drones)
	MARK_FOR_REMOVAL(sentries)

	for (int index = 0; index <= Interfaces::entityList->GetHighestEntityIndex(); index++) {
		if (localPlayerIndex == index)
			continue;

		CBaseEntity* entity = Interfaces::entityList->GetClientEntity(index);
		if (!entity)
			continue;

		if ((*entity->Origin() - localPlayer->origin).LengthSquared() > (float)(maxDistance * maxDistance))
			continue;

		const CBaseHandle handle = entity->GetRefEHandle();

		if (handle == invalidEHandleIndex)
			continue;

		if (localPlayer->observerMode == ObserverMode::OBS_MODE_IN_EYE && localPlayer->observerTarget == handle)
			continue;

		ClientClass* clientClass = entity->GetClientClass();

		if (entity->IsPlayer()) {
			auto* player = static_cast<CBasePlayer*>(entity);
			const TeamID team = *player->Team();
			if (team != TeamID::TEAM_UNASSIGNED) {
				if (player->IsAlive()) {
					if (capturePlayers)
						UpdateEntity<Player, CBasePlayer*>(players, player, index, handle, clientClass);
				} else {
					if (captureSpectators)
						UpdateEntity<Spectator, CBasePlayer*>(spectators, player, index, handle, clientClass);
				}
			}
			continue;
		} else {
			if (entity->GetDormant())
				continue;

			if (entity->IsWeaponWorldModel()) {
				if (captureWeapons)
					UpdateEntity<Weapon, CBaseCombatWeapon*>(weapons, static_cast<CBaseCombatWeapon*>(entity), index,
						handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CPlantedC4) {
				if (captureBombs)
					UpdateEntity<PlantedC4, CPlantedC4*>(bombs, static_cast<CPlantedC4*>(entity), index, handle,
						clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CHostage) {
				if (captureHostages)
					UpdateEntity<Hostage, CHostage*>(hostages, static_cast<CHostage*>(entity), index, handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CPhysPropLootCrate) {
				if (captureLootCrates)
					UpdateEntity<LootCrate, CPhysPropLootCrate*>(lootCrates, static_cast<CPhysPropLootCrate*>(entity), index, handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CDrone) {
				if (captureDrones)
					UpdateEntity<Drone, CDrone*>(drones, static_cast<CDrone*>(entity), index, handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CDronegun) {
				if (captureSentries)
					UpdateEntity<Sentry, CDronegun*>(sentries, static_cast<CDronegun*>(entity), index, handle, clientClass);
				continue;
			} else if (clientClass->m_ClassID == ClientClassID::CPhysPropAmmoBox) {
				if (captureAmmoBoxes)
					UpdateEntity<Entity, CBaseEntity*>(ammoBoxes, entity, index, handle, clientClass);
				continue;
			} else if (std::find(projectileClassIDs.begin(), projectileClassIDs.end(), clientClass->m_ClassID) != projectileClassIDs.end()) {
				if (captureProjectiles)
					UpdateEntity<Projectile, CBaseGrenade*>(projectiles, static_cast<CBaseGrenade*>(entity), index, handle, clientClass);
				continue;
			}
		}

		if (captureEntities)
			UpdateEntity<>(entities, entity, index, handle, clientClass);
	}

	auto IsMarkedForRemoval = [](const auto& pair) { // Remove old records
		return pair.second.markForRemoval;
	};

	std::erase_if(entities, IsMarkedForRemoval);
	std::erase_if(players, IsMarkedForRemoval);
	std::erase_if(spectators, IsMarkedForRemoval);
	std::erase_if(weapons, IsMarkedForRemoval);
	std::erase_if(hostages, IsMarkedForRemoval);
	std::erase_if(projectiles, IsMarkedForRemoval);
	std::erase_if(bombs, IsMarkedForRemoval);
	std::erase_if(ammoBoxes, IsMarkedForRemoval);
	std::erase_if(lootCrates, IsMarkedForRemoval);
	std::erase_if(drones, IsMarkedForRemoval);
	std::erase_if(sentries, IsMarkedForRemoval);
}