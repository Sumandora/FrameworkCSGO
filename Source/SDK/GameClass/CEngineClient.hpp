#ifndef SDK_CENGINECLIENT
#define SDK_CENGINECLIENT

#include "VirtualMethod.hpp"
#include "../../Utils/VMT.hpp"

class CEngineClient {
public:
	VIRTUAL_METHOD(12, GetLocalPlayer, int, (), (this))
	VIRTUAL_METHOD(26, IsInGame, bool, (), (this))
	// Other cheats label this pointer as a reference, this doesn't work for me
	VIRTUAL_METHOD(18, GetViewAngles, void, (Vector* angle), (this, angle))
	VIRTUAL_METHOD(20, GetMaxClients, int, (), (this))
};

#endif
