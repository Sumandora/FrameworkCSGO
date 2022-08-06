#ifndef SDK_CBASEENTITY
#define SDK_CBASEENTITY

#include "../TeamID.hpp"
#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"
#include "../Matrix3x4.hpp"
#include "xorstr.hpp"
#include "CCollideable.hpp"

class C_BaseEntity {
public:
	NETVAR_FUNCTION(TeamID,	Team,			xorstr_("DT_BaseEntity"),	xorstr_("m_iTeamNum"))
	NETVAR_FUNCTION(bool,	Spotted,		xorstr_("DT_BaseEntity"),	xorstr_("m_bSpotted"))
	NETVAR_FUNCTION(int,	SpottedByMask,	xorstr_("DT_BaseEntity"),	xorstr_("m_bSpottedByMask"))
	NETVAR_FUNCTION(Vector,	VecOrigin,		xorstr_("DT_BaseEntity"),	xorstr_("m_vecOrigin"))

	VIRTUAL_METHOD(3, GetCollideable, C_Collideable*, (), (this))

	// IClientNetworkable
	VIRTUAL_METHOD(9, GetDormant, bool, (), (this + sizeof(void*) * 2))

	// IClientRenderable
	VIRTUAL_METHOD(13, SetupBones, bool, (Matrix3x4* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0), (this + sizeof(void*), pBoneMatrix, nMaxBones, nBoneMask, flCurTime))
};

#endif
