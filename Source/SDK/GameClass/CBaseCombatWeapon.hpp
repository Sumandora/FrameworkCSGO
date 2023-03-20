#ifndef SDK_CWEAPONCSBASE
#define SDK_CWEAPONCSBASE

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "../WeaponIDs.hpp"
#include "VirtualMethod.hpp"

#include "CBaseAttributableItem.hpp"

class CBaseCombatWeapon : public CBaseAttributableItem {
public:
	NETVAR_FUNCTION(float, NextPrimaryAttack, ClientClassID::CBaseCombatWeapon, xorstr_("DT_LocalActiveWeaponData"), xorstr_("m_flNextPrimaryAttack"))
	NETVAR_FUNCTION(float, NextSecondaryAttack, ClientClassID::CBaseCombatWeapon, xorstr_("DT_LocalActiveWeaponData"), xorstr_("m_flNextSecondaryAttack"))
	NETVAR_FUNCTION(int, Ammo, ClientClassID::CBaseCombatWeapon, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_iClip1"))
	NETVAR_FUNCTION(int, ReserveAmmoCount, ClientClassID::CBaseCombatWeapon, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_iPrimaryReserveAmmoCount"))

	VIRTUAL_METHOD(529, GetCSWpnData, void*, (), (this))
};

#endif
