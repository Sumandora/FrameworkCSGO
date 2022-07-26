#ifndef SDK_CENGINECLIENT
#define SDK_CENGINECLIENT

#include "VirtualMethod.hpp"

struct CEngineClient {
	VIRTUAL_METHOD(12, GetLocalPlayer, int, (), (this))
};

#endif
