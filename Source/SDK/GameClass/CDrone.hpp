#ifndef SDK_GAMECLASS_CDRONE
#define SDK_GAMECLASS_CDRONE

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CDrone : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, MoveToThisEntity, ClientClassID::CDrone, xorstr_("DT_Drone"), xorstr_("m_hMoveToThisEntity"))
};

#endif
