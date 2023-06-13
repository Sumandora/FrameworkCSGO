#ifndef SDK_GAMECLASS_CC4
#define SDK_GAMECLASS_CC4

#include "../../Netvars.hpp"
#include "CBaseCombatWeapon.hpp"
#include "xorstr.hpp"

class CC4 : public CBaseCombatWeapon {
public:
	NETVAR_FUNCTION(bool, StartedArming, ClientClassID::CC4, xorstr_("DT_WeaponC4"), xorstr_("m_bStartedArming"))
};

#endif
