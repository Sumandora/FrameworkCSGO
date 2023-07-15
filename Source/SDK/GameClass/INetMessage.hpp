#ifndef SDK_GAMECLASS_INETMESSAGE
#define SDK_GAMECLASS_INETMESSAGE

#include "VirtualMethod.hpp"

class INetMessage {
public:
	VIRTUAL_METHOD(10, GetName, const char*, (), (this))
};

#endif