#ifndef SDK_CBASEENTITY
#define SDK_CBASEENTITY

#include "../TeamID.hpp"
#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"
#include "../Vector.hpp"
#include "../Matrix3x4.hpp"

class C_BaseEntity {
public:
	NETVAR_FUNCTION(TeamID, Team, "DT_BaseEntity", "m_iTeamNum")
	NETVAR_FUNCTION(bool, Spotted, "DT_BaseEntity", "m_bSpotted")
	NETVAR_FUNCTION(int, SpottedByMask, "DT_BaseEntity", "m_bSpottedByMask")
	NETVAR_FUNCTION(Vector, VecOrigin, "DT_BaseEntity", "m_vecOrigin")

	// IClientNetworkable
	VIRTUAL_METHOD(9, GetDormant, bool, (), (this + sizeof(void*) * 2))

	// IClientRenderable
	VIRTUAL_METHOD(13, SetupBones, bool, (matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0), (this + sizeof(void*), pBoneMatrix, nMaxBones, nBoneMask, flCurTime))
};

#endif
