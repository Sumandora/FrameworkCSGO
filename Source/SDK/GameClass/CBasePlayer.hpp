#ifndef SDK_CBASEPLAYER
#define SDK_CBASEPLAYER

#include "CBaseEntity.hpp"
#include "CWeaponCSBase.hpp"

#include "../Definitions/LifeState.hpp"
#include "../Definitions/Bones.hpp"

class CBasePlayer : public CBaseEntity {
public:
	NETVAR_FUNCTION(int,	Flags,				xorstr_("DT_BasePlayer"),				xorstr_("m_fFlags"))
	NETVAR_FUNCTION(char,	LifeState,			xorstr_("DT_BasePlayer"),				xorstr_("m_lifeState"))
	NETVAR_FUNCTION(bool,	GunGameImmunity,	xorstr_("DT_CSPlayer"),					xorstr_("m_bGunGameImmunity"))
	NETVAR_FUNCTION(Vector,	VecViewOffset,		xorstr_("DT_LocalPlayerExclusive"),		xorstr_("m_vecViewOffset[0]"))

	// This can be more than just a weaponcsbase, means that I can't enforce a cast to weaponcsbase here
	NETVAR_FUNCTION(void*,	ActiveWeapon,		xorstr_("DT_BaseCombatCharacter"),		xorstr_("m_hActiveWeapon"))

	COMBINED_NETVAR_FUNCTION(Vector, AimPunchAngle,		xorstr_("DT_LocalPlayerExclusive"),	xorstr_("m_Local"),	xorstr_("DT_Local"),	xorstr_("m_aimPunchAngle"))
	COMBINED_NETVAR_FUNCTION(Vector, AimPunchAngleVel,	xorstr_("DT_LocalPlayerExclusive"),	xorstr_("m_Local"),	xorstr_("DT_Local"),	xorstr_("m_aimPunchAngleVel"))

	NETVAR_FUNCTION(int,	ShotsFired,			xorstr_("DT_CSLocalPlayerExclusive"),	xorstr_("m_iShotsFired"))
	NETVAR_FUNCTION(int,	Health,			xorstr_("DT_BasePlayer"),	xorstr_("m_iHealth"))

	inline Vector GetEyePosition() {
		return *this->VecOrigin() + *this->VecViewOffset();
	}

	//TODO Split SetupBones and BonePosition
	inline Vector GetBonePosition(int boneIndex) {
		Matrix3x4 boneMatrix[MAXSTUDIOBONES];

		if (!SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return Vector {};

		Matrix3x4 hitbox = boneMatrix[boneIndex];

		return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}
};

#endif
