#ifndef SDK_GAMECLASS_ENTITIES_CBASECOMBATWEAPON
#define SDK_GAMECLASS_ENTITIES_CBASECOMBATWEAPON

#include "../../../Netvars.hpp"
#include "../../Weapons/WeaponIDs.hpp"
#include "../VirtualMethod.hpp"

#include "CBaseAttributableItem.hpp"

class CBaseCombatWeapon : public CBaseAttributableItem {
public:
	NETVAR_FUNCTION(float, NextPrimaryAttack, ClientClassID::CBaseCombatWeapon, "DT_LocalActiveWeaponData", "m_flNextPrimaryAttack")
	NETVAR_FUNCTION(float, NextSecondaryAttack, ClientClassID::CBaseCombatWeapon, "DT_LocalActiveWeaponData", "m_flNextSecondaryAttack")
	NETVAR_FUNCTION(int, Ammo, ClientClassID::CBaseCombatWeapon, "DT_BaseCombatWeapon", "m_iClip1")
	NETVAR_FUNCTION(int, ReserveAmmoCount, ClientClassID::CBaseCombatWeapon, "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount")

	VIRTUAL_METHOD(529, GetCSWpnData, void*, (), (this))
};

#endif
