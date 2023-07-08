#ifndef SDK_GAMECLASS_CDRONE
#define SDK_GAMECLASS_CDRONE

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CDrone : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, MoveToThisEntity, ClientClassID::CDrone, "DT_Drone", "m_hMoveToThisEntity")
};

#endif
