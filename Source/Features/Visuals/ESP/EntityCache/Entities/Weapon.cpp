#include "../EntityCache.hpp"

void Weapon::Update(CBaseCombatWeapon* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	weaponID = *entity->WeaponDefinitionIndex();
	ammo = *entity->Ammo();
	reserveAmmoCount = *entity->ReserveAmmoCount();
	ownerEntity = *entity->OwnerEntity();
}
