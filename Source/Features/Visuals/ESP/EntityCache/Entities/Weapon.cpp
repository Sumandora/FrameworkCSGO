#include "../EntityCache.hpp"

void Weapon::update(CBaseCombatWeapon* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	weaponID = *entity->WeaponDefinitionIndex();
	ammo = *entity->Ammo();
	reserveAmmoCount = *entity->ReserveAmmoCount();
	ownerEntity = *entity->OwnerEntity();
}
