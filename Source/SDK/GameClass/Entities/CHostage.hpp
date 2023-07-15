#ifndef SDK_GAMECLASS_ENTITIES_CHOSTAGE
#define SDK_GAMECLASS_ENTITIES_CHOSTAGE

#include "CBaseEntity.hpp"

#include <map>
#include <optional>

class CHostage : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, HostageState, ClientClassID::CHostage, "DT_CHostage", "m_nHostageState")
	NETVAR_FUNCTION(float, GrabSuccessTime, ClientClassID::CHostage, "DT_CHostage", "m_flGrabSuccessTime")
};

#endif
