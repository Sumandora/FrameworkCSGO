#ifndef SDK_GAMECLASS_ENTITIES_CC4
#define SDK_GAMECLASS_ENTITIES_CC4

#include "../../../Netvars.hpp"
#include "CBaseCombatWeapon.hpp"

class CC4 : public CBaseCombatWeapon {
public:
	NETVAR_FUNCTION(bool, StartedArming, ClientClassID::CC4, "DT_WeaponC4", "m_bStartedArming")
};

#endif
