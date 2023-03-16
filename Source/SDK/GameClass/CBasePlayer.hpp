#ifndef SDK_CBASEPLAYER
#define SDK_CBASEPLAYER

#include "CBaseCombatWeapon.hpp"
#include "CBaseEntity.hpp"

#include "../Definitions/Bones.hpp"
#include "../Definitions/LifeState.hpp"
#include "../ObserverMode.hpp"

#include "../../ConVarStorage.hpp"
#include "../../GameCache.hpp"

#include "../../Utils/GameMode.hpp"

#include "../Math/Vector.hpp"

class CBasePlayer : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Flags, xorstr_("DT_BasePlayer"), xorstr_("m_fFlags"))
	NETVAR_FUNCTION(char, LifeState, xorstr_("DT_BasePlayer"), xorstr_("m_lifeState"))
	NETVAR_FUNCTION(bool, GunGameImmunity, xorstr_("DT_CSPlayer"), xorstr_("m_bGunGameImmunity"))
	NETVAR_FUNCTION(Vector, VecViewOffset, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecViewOffset[0]"))
	NETVAR_FUNCTION(bool, SurvivalTeam, xorstr_("DT_CSPlayer"), xorstr_("m_nSurvivalTeam"))

	// Use CClientEntityList::GetClientEntityFromHandle to get the weapon itself
	NETVAR_FUNCTION(void*, ActiveWeapon, xorstr_("DT_BaseCombatCharacter"), xorstr_("m_hActiveWeapon"))
	NETVAR_FUNCTION(bool, WaitForNoAttack, xorstr_("DT_CSPlayer"), xorstr_("m_bWaitForNoAttack"))

	COMBINED_NETVAR_FUNCTION(Vector, AimPunchAngle, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_Local"), xorstr_("DT_Local"), xorstr_("m_aimPunchAngle"))

	NETVAR_FUNCTION(int, ShotsFired, xorstr_("DT_CSLocalPlayerExclusive"), xorstr_("m_iShotsFired"))
	NETVAR_FUNCTION(int, Health, xorstr_("DT_BasePlayer"), xorstr_("m_iHealth"))

	NETVAR_FUNCTION(enum ObserverMode, ObserverMode, xorstr_("DT_BasePlayer"), xorstr_("m_iObserverMode"))
	NETVAR_FUNCTION(void, ObserverTarget, xorstr_("DT_BasePlayer"), xorstr_("m_hObserverTarget"))

	NETVAR_FUNCTION(unsigned int, TickBase, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_nTickBase"))

	NETVAR_FUNCTION(float, FlashMaxAlpha, xorstr_("DT_CSPlayer"), xorstr_("m_flFlashMaxAlpha"))

	NETVAR_FUNCTION(Vector, Velocity, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecVelocity[0]"))

	inline Vector GetEyePosition()
	{
		return *this->VecOrigin() + *this->VecViewOffset();
	}

	inline bool SetupBones(Matrix3x4 (&boneMatrix)[])
	{
		return CBaseEntity::SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Memory::globalVars->curtime);
	}

	inline bool IsEnemy()
	{
		CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
		if (localPlayer != this) {
			if (ConVarStorage::mp_teammates_are_enemies->GetBool())
				return true;
			else {
				if (Utils::CalculateGamemode() == Gamemode::DANGER_ZONE) {
					const int localSurvivalTeam = *localPlayer->SurvivalTeam();
					if (localSurvivalTeam < 0) // DZ without teams
						return true;

					return localSurvivalTeam != *SurvivalTeam();
				} else
					return *localPlayer->Team() != *Team();
			}
		}
		return false;
	}
};

#endif
