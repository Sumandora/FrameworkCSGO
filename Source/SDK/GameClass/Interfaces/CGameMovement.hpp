#ifndef SDK_CGAMEMOVEMENT
#define SDK_CGAMEMOVEMENT

#include "../CBasePlayer.hpp"
#include "../CMoveData.hpp"
#include "../VirtualMethod.hpp"

class CGameMovement {
public:
    VIRTUAL_METHOD(2, ProcessMovement, void, (CBasePlayer* pPlayer, CMoveData* pMove), (this, pPlayer, pMove))
    VIRTUAL_METHOD(4, StartTrackpredictionErrors, void, (CBasePlayer* pPlayer), (this, pPlayer))
    VIRTUAL_METHOD(5, FinishTrackPredictionErrors, void, (CBasePlayer* pPlayer), (this, pPlayer))
};

#endif