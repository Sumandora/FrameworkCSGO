#ifndef SDK_CBASEENTITY
#define SDK_CBASEENTITY

#include "CCollideable.hpp"
#include "VirtualMethod.hpp"
#include "xorstr.hpp"

#include "../Math/Matrix3x4.hpp"
#include "../Netvars/ClientClass.hpp"
#include "../TeamID.hpp"

#include "../../Netvars.hpp"

class CBaseEntity {
public:
	NETVAR_FUNCTION(TeamID, Team, xorstr_("DT_BaseEntity"), xorstr_("m_iTeamNum"))
	NETVAR_FUNCTION(bool, Spotted, xorstr_("DT_BaseEntity"), xorstr_("m_bSpotted"))
	NETVAR_FUNCTION(int, SpottedByMask, xorstr_("DT_BaseEntity"), xorstr_("m_bSpottedByMask"))
	NETVAR_FUNCTION(Vector, VecOrigin, xorstr_("DT_BaseEntity"), xorstr_("m_vecOrigin"))

	NETVAR_FUNCTION(CCollideable, Collision, xorstr_("DT_BaseEntity"), xorstr_("m_Collision"))

	NETVAR_FUNCTION(float, SimulationTime, xorstr_("DT_BaseEntity"), xorstr_("m_flSimulationTime"))
	NETVAR_FUNCTION(int, OwnerEntity, xorstr_("DT_BaseEntity"), xorstr_("m_hOwnerEntity"))

	// https://github.com/danielkrupinski/Osiris/blob/444d7dd9daeb0f46de412dbb524c91dfdedf1723/Source/CSGO/Entity.h#L79
	//  IClientNetworkable
	VIRTUAL_METHOD(2, GetClientClass, ClientClass*, (), (this + sizeof(void*) * 2))
	VIRTUAL_METHOD(9, GetDormant, bool, (), (this + sizeof(void*) * 2))
	VIRTUAL_METHOD(10, entindex, int, (), (this + sizeof(void*) * 2))

	// IClientRenderable
	VIRTUAL_METHOD(13, SetupBones, bool, (Matrix3x4 * pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0), (this + sizeof(void*), pBoneMatrix, nMaxBones, nBoneMask, flCurTime))

	// Actually CBaseEntity
	VIRTUAL_METHOD(210, IsPlayer, bool, (), (this))
	VIRTUAL_METHOD(218, IsWeapon, bool, (), (this))
};

#endif
