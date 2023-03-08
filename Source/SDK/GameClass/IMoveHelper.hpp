#ifndef SDK_IMOVEHELPER
#define SDK_IMOVEHELPER

#include "CBaseEntity.hpp"
#include "VirtualMethod.hpp"

class IMoveHelper {
public:
	VIRTUAL_METHOD(1, SetHost, void, (CBaseEntity * player), (this, player))
};

#endif