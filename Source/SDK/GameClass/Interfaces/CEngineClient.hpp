#ifndef SDK_GAMECLASS_INTERFACES_CENGINECLIENT
#define SDK_GAMECLASS_INTERFACES_CENGINECLIENT

#include "../VirtualMethod.hpp"

#include "../CNetChan.hpp"

#include "../../PlayerInfo.hpp"

#include "../../Math/Matrix4x4.hpp"
#include "../../Math/Vector.hpp"

class CEngineClient {
public:
	static constexpr int worldToScreenIndex = 37;
	VIRTUAL_METHOD(8, GetPlayerInfo, bool, (int index, PlayerInfo* playerInfo), (this, index, playerInfo))
	VIRTUAL_METHOD(12, GetLocalPlayer, int, (), (this))
	VIRTUAL_METHOD(18, GetViewAngles, void, (Vector * angle), (this, angle))
	VIRTUAL_METHOD(19, SetViewAngles, void, (Vector * angle), (this, angle))
	VIRTUAL_METHOD(20, GetMaxClients, int, (), (this))
	VIRTUAL_METHOD(26, IsInGame, bool, (), (this))
	VIRTUAL_METHOD(worldToScreenIndex, WorldToScreenMatrix, Matrix4x4*, (), (this))
	VIRTUAL_METHOD(78, GetNetChannel, CNetChan*, (), (this))
};

#endif
