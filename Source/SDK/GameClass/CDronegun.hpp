#ifndef SDK_GAMECLASS_CDRONEGUN
#define SDK_GAMECLASS_CDRONEGUN

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CDronegun : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Health, ClientClassID::CDronegun, xorstr_("DT_Dronegun"), xorstr_("m_iHealth"))
};

#endif
