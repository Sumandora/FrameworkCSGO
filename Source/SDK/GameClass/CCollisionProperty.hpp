#ifndef SDK_GAMECLASS_CCOLLISIONPROPERTY
#define SDK_GAMECLASS_CCOLLISIONPROPERTY

#include "../../Netvars.hpp"
#include "../Math/Vector.hpp"

class CCollisionProperty {
public:
	NETVAR_FUNCTION(Vector, VecMins, ClientClassID::CBaseEntity, "DT_CollisionProperty", "m_vecMins")
	NETVAR_FUNCTION(Vector, VecMaxs, ClientClassID::CBaseEntity, "DT_CollisionProperty", "m_vecMaxs")
};

#endif
