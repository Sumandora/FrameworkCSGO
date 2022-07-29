#ifndef SDK_CBASEENTITY
#define SDK_CBASEENTITY

#include "../TeamID.hpp"
#include "../../Netvars.hpp"
#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "../Vector.hpp"

struct C_BaseEntity : IClientNetworkable, IClientRenderable {
public:
	NETVAR_FUNCTION(TeamID, Team, "DT_BaseEntity", "m_iTeamNum")
	NETVAR_FUNCTION(bool, Spotted, "DT_BaseEntity", "m_bSpotted")
	NETVAR_FUNCTION(int, SpottedByMask, "DT_BaseEntity", "m_bSpottedByMask")
	NETVAR_FUNCTION(Vector, VecOrigin, "DT_BaseEntity", "m_vecOrigin")
};

#endif
