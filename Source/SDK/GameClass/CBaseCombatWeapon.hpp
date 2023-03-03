#ifndef SDK_CWEAPONCSBASE
#define SDK_CWEAPONCSBASE

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

class CBaseCombatWeapon {
public:
	NETVAR_FUNCTION(float, NextPrimaryAttack, xorstr_("DT_BaseCombatWeapon"), xorstr_("m_flNextPrimaryAttack"))

	VIRTUAL_METHOD(529, GetCSWpnData, void*, (), (this))
};

#endif
