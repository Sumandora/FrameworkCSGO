#include "../EntityCache.hpp"

static std::map<ClientClassID, ProjectileType> projectileMap{
	{ ClientClassID::CBreachChargeProjectile, ProjectileType::BREACH_CHARGE },
	{ ClientClassID::CBumpMineProjectile, ProjectileType::BUMP_MINE },
	{ ClientClassID::CDecoyProjectile, ProjectileType::DECOY },
	{ ClientClassID::CMolotovProjectile, ProjectileType::MOLOTOV },
	{ ClientClassID::CSensorGrenadeProjectile, ProjectileType::SENSOR_GRENADE },
	{ ClientClassID::CSmokeGrenadeProjectile, ProjectileType::SMOKE_GRENADE },
	{ ClientClassID::CSnowballProjectile, ProjectileType::SNOWBALL },
};

void Projectile::update(CBaseGrenade* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	thrower = *entity->Thrower();

	trail.push_back(*entity->Origin());

	if (clientClass->m_ClassID == ClientClassID::CBaseCSGrenadeProjectile) {
		if (strstr(entity->GetModel()->szPathName, "flashbang"))
			type = ProjectileType::FLASHBANG;
		else
			type = ProjectileType::HIGH_EXPLOSIVE_GRENADE;
	} else if (projectileMap.contains(clientClass->m_ClassID)) {
		type = projectileMap[clientClass->m_ClassID];
	} else {
		type = ProjectileType::INVALID;
		__asm("int3");
	}
}
