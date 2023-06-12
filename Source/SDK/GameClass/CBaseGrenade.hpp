#ifndef SDK_GAMECLASS_CBASEGRENADE
#define SDK_GAMECLASS_CBASEGRENADE

#include "../../Netvars.hpp"
#include "CBaseEntity.hpp"
#include "xorstr.hpp"

#include "../CBaseHandle.hpp"

class CBaseGrenade : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, Thrower, ClientClassID::CBaseGrenade, xorstr_("DT_BaseGrenade"), xorstr_("m_hThrower"))
};

#endif
