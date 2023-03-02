#ifndef SDK_CNETCHAN
#define SDK_CNETCHAN

#include "VirtualMethod.hpp"

class CNetChan {
public:
	VIRTUAL_METHOD(9, GetLatency, float, (int flow), (this, flow))
};

#endif
