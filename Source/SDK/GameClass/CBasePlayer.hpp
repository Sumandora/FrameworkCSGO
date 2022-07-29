#ifndef SDK_CBASEPLAYER
#define SDK_CBASEPLAYER

#include "CBaseEntity.hpp"
#include "../LifeState.hpp"
#include "../BoneDefinitions.hpp"

class C_BasePlayer : public C_BaseEntity {
public:
	NETVAR_FUNCTION(int, Flags, "DT_BasePlayer", "m_fFlags")
	NETVAR_FUNCTION(char, LifeState, "DT_BasePlayer", "m_lifeState")
	NETVAR_FUNCTION(bool, GunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity")
	NETVAR_FUNCTION(Vector, VecViewOffset, "DT_LocalPlayerExclusive", "m_vecViewOffset[0]")

	inline Vector GetEyePosition() {
		return *this->VecOrigin() + *this->VecViewOffset();
	}

	//TODO Split SetupBones and BonePosition
	inline Vector GetBonePosition(int boneIndex) {
		matrix3x4_t boneMatrix[MAXSTUDIOBONES];

		if (!SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return Vector {};

		matrix3x4_t hitbox = boneMatrix[boneIndex];

		return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}
};

#endif
