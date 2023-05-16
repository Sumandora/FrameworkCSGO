#ifndef SDK_GAMECLASS_INTERFACES_CCLIENTENTITYLIST
#define SDK_GAMECLASS_INTERFACES_CCLIENTENTITYLIST

#include "SDK/GameClass//CBaseEntity.hpp"
#include "SDK/GameClass//VirtualMethod.hpp"

class CClientEntityList {
public:
	VIRTUAL_METHOD(3, GetClientEntity, CBaseEntity*, (int index), (this, index))
	VIRTUAL_METHOD(4, GetClientEntityFromHandle, CBaseEntity*, (CBaseHandle* handle), (this, handle))
	VIRTUAL_METHOD(6, GetHighestEntityIndex, int, (), (this))
};

#endif
