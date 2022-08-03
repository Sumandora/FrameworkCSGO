#ifndef SDK_CWEAPONCSBASE
#define SDK_CWEAPONCSBASE

#include "../../Netvars.hpp"
#include "../WeaponMode.hpp"

#include "xorstr.hpp"

class C_WeaponCSBase {
public:
	NETVAR_FUNCTION(CSWeaponMode,	WeaponMode,	xorstr_("DT_WeaponCSBase"),	xorstr_("m_weaponMode"))
};

#endif
