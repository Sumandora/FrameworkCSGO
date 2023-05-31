#ifndef SDK_GAMECLASS_CCOLLISIONPROPERTY
#define SDK_GAMECLASS_CCOLLISIONPROPERTY

#include "../../Netvars.hpp"
#include "../Math/Vector.hpp"

class CCollisionProperty {
public:
	NETVAR_FUNCTION(Vector, VecMins, ClientClassID::CBaseEntity, xorstr_("DT_CollisionProperty"), xorstr_("m_vecMins"))
	NETVAR_FUNCTION(Vector, VecMaxs, ClientClassID::CBaseEntity, xorstr_("DT_CollisionProperty"), xorstr_("m_vecMaxs"))
};

#endif
