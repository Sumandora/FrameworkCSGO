#ifndef SDK_GAMECLASS_INTERFACES_CGAMEMOVEMENT
#define SDK_GAMECLASS_INTERFACES_CGAMEMOVEMENT

#include "SDK/GameClass/CBasePlayer.hpp"
#include "SDK/GameClass/CMoveData.hpp"
#include "SDK/GameClass/VirtualMethod.hpp"

class CGameMovement {
public:
	VIRTUAL_METHOD(2, ProcessMovement, void, (CBasePlayer * pPlayer, CMoveData* pMove), (this, pPlayer, pMove))
	VIRTUAL_METHOD(4, StartTrackPredictionErrors, void, (CBasePlayer * pPlayer), (this, pPlayer))
	VIRTUAL_METHOD(5, FinishTrackPredictionErrors, void, (CBasePlayer * pPlayer), (this, pPlayer))
};

#endif