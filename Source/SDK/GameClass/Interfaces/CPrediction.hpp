#ifndef SDK_GAMECLASS_INTERFACES_CPREDICTION
#define SDK_GAMECLASS_INTERFACES_CPREDICTION

#include "../CMoveData.hpp"
#include "../CUserCmd.hpp"
#include "../Entities/CBasePlayer.hpp"
#include "../IMoveHelper.hpp"
#include "../Padding.hpp"

class CPrediction {
public:
	PADDING(12);
	bool m_bInPrediction; // TODO maybe use vfuncs?
	bool m_bIsFirstTimePredicted;

	VIRTUAL_METHOD(21, SetupMove, void, (CBasePlayer * entity, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move), (this, entity, ucmd, pHelper, move))
	VIRTUAL_METHOD(22, FinishMove, void, (CBasePlayer * entity, CUserCmd* ucmd, CMoveData* move), (this, entity, ucmd, move))
};

#endif