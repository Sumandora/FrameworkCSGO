#ifndef SDK_GAMECLASS_INTERFACES_IPREDICTION
#define SDK_GAMECLASS_INTERFACES_IPREDICTION

#include "SDK/GameClass/CBasePlayer.hpp"
#include "SDK/GameClass/CMoveData.hpp"
#include "SDK/GameClass/CUserCmd.hpp"
#include "SDK/GameClass/IMoveHelper.hpp"

class IPrediction {
public:
	VIRTUAL_METHOD(21, SetupMove, void, (CBasePlayer * entity, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move), (this, entity, ucmd, pHelper, move))
	VIRTUAL_METHOD(22, FinishMove, void, (CBasePlayer * entity, CUserCmd* ucmd, CMoveData* move), (this, entity, ucmd, move))
};

#endif