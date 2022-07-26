#ifndef SDK_CCLIENTENTITYLIST
#define SDK_CCLIENTENTITYLIST

#include "VirtualMethod.hpp"

struct CClientEntityList {
	VIRTUAL_METHOD(3, GetClientEntity, void*, (int index), (this, index))
};

#endif
