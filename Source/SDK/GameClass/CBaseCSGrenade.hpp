#ifndef SDK_GAMECLASS_CBASECSGRENADE
#define SDK_GAMECLASS_CBASECSGRENADE

#include "../../Netvars.hpp"
#include "CBaseEntity.hpp"
#include "xorstr.hpp"

#include "CBaseCombatWeapon.hpp"

class CBaseCSGrenade : public CBaseCombatWeapon {
public:
	NETVAR_FUNCTION(CBaseHandle, PinPulled, ClientClassID::CBaseCSGrenade, xorstr_("DT_BaseCSGrenade"), xorstr_("m_bPinPulled"))
};

#endif
