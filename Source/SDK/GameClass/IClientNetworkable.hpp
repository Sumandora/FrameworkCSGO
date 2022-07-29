#ifndef SDK_ICLIENTNETWORKABLE
#define SDK_ICLIENTNETWORKABLE

#include "VirtualMethod.hpp"

struct IClientNetworkable {
public:
	VIRTUAL_METHOD(9, GetDormant, bool, (), (this + sizeof(uintptr_t) * 2))
};

#endif
