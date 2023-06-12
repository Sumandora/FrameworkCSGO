#ifndef SDK_GAMECLASS_CBASECSGRENADE
#define SDK_GAMECLASS_CBASECSGRENADE

#include "../../Netvars.hpp"
#include "CBaseEntity.hpp"
#include "xorstr.hpp"

#include "../CBaseHandle.hpp"

class CBaseCSGrenade : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, PinPulled, ClientClassID::CBaseCSGrenade, xorstr_("DT_BaseCSGrenade"), xorstr_("m_bPinPulled"))
};

#endif
