#ifndef SDK_CHOSTAGE
#define SDK_CHOSTAGE

#include "CBaseEntity.hpp"

#include <map>
#include <optional>

class CHostage : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, HostageState, ClientClassID::CHostage, xorstr_("DT_CHostage"), xorstr_("m_nHostageState"))
	NETVAR_FUNCTION(float, GrabSuccessTime, ClientClassID::CHostage, xorstr_("DT_CHostage"), xorstr_("m_flGrabSuccessTime"))
};

#endif
