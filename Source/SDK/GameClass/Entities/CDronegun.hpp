#ifndef SDK_GAMECLASS_ENTITIES_CDRONEGUN
#define SDK_GAMECLASS_ENTITIES_CDRONEGUN

#include "../../../Netvars.hpp"
#include "../VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CDronegun : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Health, ClientClassID::CDronegun, "DT_Dronegun", "m_iHealth")
};

#endif
