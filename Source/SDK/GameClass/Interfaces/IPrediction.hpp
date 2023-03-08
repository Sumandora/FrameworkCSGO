#ifndef SDK_IPREDICTION
#define SDK_IPREDICTION

#include "../../CUserCmd.hpp"
#include "../CBasePlayer.hpp"
#include "../CMoveData.hpp"
#include "../IMoveHelper.hpp"

class IPrediction {
public:
	VIRTUAL_METHOD(21, SetupMove, void, (CBasePlayer * entity, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move), (this, entity, ucmd, pHelper, move))
	VIRTUAL_METHOD(22, FinishMove, void, (CBasePlayer * entity, CUserCmd* ucmd, CMoveData* move), (this, entity, ucmd, move))
};

#endif