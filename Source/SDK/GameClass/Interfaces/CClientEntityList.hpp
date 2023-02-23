#ifndef SDK_CCLIENTENTITYLIST
#define SDK_CCLIENTENTITYLIST

#include "../VirtualMethod.hpp"
#include "../CBaseEntity.hpp"

class CClientEntityList {
public:
	VIRTUAL_METHOD(3, GetClientEntity, CBaseEntity*, (int index), (this, index))
};

#endif
