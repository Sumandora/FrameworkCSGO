#ifndef SDK_GAMECLASS_IMOVEHELPER
#define SDK_GAMECLASS_IMOVEHELPER

#include "CBaseEntity.hpp"
#include "VirtualMethod.hpp"

class IMoveHelper {
public:
	VIRTUAL_METHOD(1, SetHost, void, (CBaseEntity * player), (this, player))
};

#endif