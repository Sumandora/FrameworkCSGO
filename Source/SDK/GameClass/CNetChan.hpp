#ifndef SDK_GAMECLASS_CNETCHAN
#define SDK_GAMECLASS_CNETCHAN

#include "VirtualMethod.hpp"

class CNetChan {
public:
	VIRTUAL_METHOD(9, GetLatency, float, (int flow), (this, flow))
};

#endif
