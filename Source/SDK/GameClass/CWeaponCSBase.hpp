#ifndef SDK_CWEAPONCSBASE
#define SDK_CWEAPONCSBASE

#include "xorstr.hpp"

#include "../WeaponMode.hpp"

#include "../../Netvars.hpp"

class CWeaponCSBase {
public:
	NETVAR_FUNCTION(CSWeaponMode,	WeaponMode,	xorstr_("DT_WeaponCSBase"),	xorstr_("m_weaponMode"))
};

#endif
