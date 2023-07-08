#ifndef SDK_GAMECLASS_CBASECSGRENADE
#define SDK_GAMECLASS_CBASECSGRENADE

#include "../../Netvars.hpp"
#include "CBaseCombatWeapon.hpp"
#include "CBaseEntity.hpp"

class CBaseCSGrenade : public CBaseCombatWeapon {
public:
	NETVAR_FUNCTION(CBaseHandle, PinPulled, ClientClassID::CBaseCSGrenade, "DT_BaseCSGrenade", "m_bPinPulled")
};

#endif
