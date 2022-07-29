#ifndef SDK_CCLIENTENTITYLIST
#define SDK_CCLIENTENTITYLIST

#include "VirtualMethod.hpp"
#include "CBaseEntity.hpp"

struct CClientEntityList {
public:
	VIRTUAL_METHOD(3, GetClientEntity, C_BaseEntity*, (int index), (this, index))
};

#endif
