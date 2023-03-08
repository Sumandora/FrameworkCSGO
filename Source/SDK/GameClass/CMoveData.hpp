#ifndef SDK_CMOVEDATA
#define SDK_CMOVEDATA

#include "../EntityHandle.hpp"
#include "../Math/Vector.hpp"

class CMoveData {
public:
	bool m_bFirstRunOfFunctions;
	bool m_bGameCodeMovedPlayer;
	bool m_bNoAirControl;

	EntityHandle m_nPlayerHandle;

	int m_nImpulseCommand;
	Vector m_vecViewAngles;
	Vector m_vecAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	Vector m_vecVelocity;
	Vector m_vecTrailingVelocity;
	float m_flTrailingVelocityTime;
	Vector m_vecAngles;
	Vector m_vecOldAngles;

	float m_outStepHeight;
	Vector m_outWishVel;
	Vector m_outJumpVel;

	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	bool m_bConstraintPastRadius;

	// private:
	Vector m_vecAbsOrigin;
};

#endif
