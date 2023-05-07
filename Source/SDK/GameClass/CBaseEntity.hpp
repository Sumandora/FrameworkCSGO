#ifndef SDK_CBASEENTITY
#define SDK_CBASEENTITY

#include "CCollideable.hpp"
#include "VirtualMethod.hpp"
#include "xorstr.hpp"

#include "../Definitions/Bones.hpp"
#include "../Math/Matrix3x4.hpp"
#include "../Model/Model.hpp"
#include "../Netvars/ClientClass.hpp"
#include "../TeamID.hpp"

#include "../../Netvars.hpp"
#include "../Math/BoundingBox.hpp"

#include "../CBaseHandle.hpp"

#include <map>
#include <optional>

class CBaseEntity {
public:
	NETVAR_FUNCTION(TeamID, Team, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_iTeamNum"))
	NETVAR_FUNCTION(bool, Spotted, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_bSpotted"))
	NETVAR_FUNCTION(int, SpottedByMask, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_bSpottedByMask"))
	NETVAR_FUNCTION(Vector, Origin, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_vecOrigin"))

	NETVAR_FUNCTION(CCollideable, Collision, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_Collision"))

	NETVAR_FUNCTION(float, SimulationTime, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_flSimulationTime"))
	NETVAR_FUNCTION(int, OwnerEntity, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_hOwnerEntity"))

	NETVAR_FUNCTION(unsigned char, RenderMode, ClientClassID::CBaseEntity, xorstr_("DT_BaseEntity"), xorstr_("m_nRenderMode"))

	// MoveType used to be a netvar but got removed, RenderMode is still there.
	unsigned char GetMoveType()
	{
		return *(RenderMode() + 1);
	}

	// TODO Separate classes

	// IClientUnknown
	VIRTUAL_METHOD(3, GetRefEHandle, CBaseHandle&, (), (this))

	// IClientNetworkable
	VIRTUAL_METHOD(2, GetClientClass, ClientClass*, (), (this + sizeof(void*) * 2))
	VIRTUAL_METHOD(9, GetDormant, bool, (), (this + sizeof(void*) * 2))
	VIRTUAL_METHOD(10, entindex, int, (), (this + sizeof(void*) * 2))

	// IClientRenderable
	VIRTUAL_METHOD(1, GetRenderOrigin, Vector&, (), (this + sizeof(void*)))
	VIRTUAL_METHOD(2, GetRenderAngles, Vector&, (), (this + sizeof(void*)))
	VIRTUAL_METHOD(8, GetModel, Model*, (), (this + sizeof(void*)))
	VIRTUAL_METHOD(13, SetupBones, bool, (Matrix3x4 * pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0), (this + sizeof(void*), pBoneMatrix, nMaxBones, nBoneMask, flCurTime))

	// Actually CBaseEntity
	VIRTUAL_METHOD(210, IsPlayer, bool, (), (this))
	VIRTUAL_METHOD(218, IsWeaponWorldModel, bool, (), (this))

	inline Matrix3x4* SetupBones()
	{
		static std::map<void*, Matrix3x4[MAXSTUDIOBONES]> cache;
		static int lastFrameCount;

		const int currFrameCount = Memory::globalVars->framecount;
		if (currFrameCount != lastFrameCount) {
			cache.clear();
			lastFrameCount = currFrameCount;
		}

		if (!cache.contains(this))
			SetupBones(cache[this], MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Memory::globalVars->curtime);

		return cache[this];
	}

	inline Vector GetBonePosition(int boneIndex)
	{
		return SetupBones()[boneIndex].Origin();
	}

	inline std::optional<BoundingBox> EntityBounds()
	{
		if (IsPlayer()) {
			CCollideable* collision = Collision();
			if(!collision)
				return std::nullopt;
			return BoundingBox{ *collision->ObbMins(), *collision->ObbMaxs() };
		} else {
			Model* model = GetModel();
			if(!model)
				return std::nullopt;
			return BoundingBox{ model->mins, model->maxs };
		}
	}
};

#endif
