#ifndef SDK_GAMECLASS_CBASEGRENADE
#define SDK_GAMECLASS_CBASEGRENADE

#include "../../Netvars.hpp"
#include "CBaseEntity.hpp"

#include "../CBaseHandle.hpp"

class CBaseGrenade : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, Thrower, ClientClassID::CBaseGrenade, "DT_BaseGrenade", "m_hThrower")
};

#endif
