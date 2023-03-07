#ifndef SDK_CWEAPONCSBASE
#define SDK_CWEAPONCSBASE

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "../WeaponIDs.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CBaseCombatWeapon : public CBaseEntity {
public:
	NETVAR_FUNCTION(float, NextPrimaryAttack, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_flNextPrimaryAttack"))
	TRIPLE_COMBINED_NETVAR_FUNCTION(WeaponID, WeaponDefinitionIndex, xorstr_("DT_BaseAttributableItem"), xorstr_("m_AttributeManager"), xorstr_("DT_AttributeContainer"), xorstr_("m_Item"), xorstr_("DT_ScriptCreatedItem"), xorstr_("m_iItemDefinitionIndex"))
	NETVAR_FUNCTION(int, Ammo, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_iClip1"))
	NETVAR_FUNCTION(int, ReserveAmmoCount, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_iPrimaryReserveAmmoCount"))

	VIRTUAL_METHOD(529, GetCSWpnData, void*, (), (this))
};

#endif
