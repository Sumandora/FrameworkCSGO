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
	NETVAR_FUNCTION(int, Flags, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_fFlags"))
	NETVAR_FUNCTION(char, LifeState, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_lifeState"))
	NETVAR_FUNCTION(bool, GunGameImmunity, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bGunGameImmunity"))
	NETVAR_FUNCTION(Vector, VecViewOffset, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecViewOffset[0]"))
	NETVAR_FUNCTION(bool, SurvivalTeam, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_nSurvivalTeam"))

	NETVAR_FUNCTION(void, ActiveWeapon, ClientClassID::CBaseCombatCharacter, xorstr_("DT_BaseCombatCharacter"), xorstr_("m_hActiveWeapon"))
	NETVAR_FUNCTION(bool, WaitForNoAttack, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bWaitForNoAttack"))

	COMBINED_NETVAR_FUNCTION(Vector, AimPunchAngle,
		ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_Local"),
		ClientClassID::CBasePlayer, xorstr_("DT_Local"), xorstr_("m_aimPunchAngle"))
	COMBINED_NETVAR_FUNCTION(Vector, ViewPunchAngle,
		ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_Local"),
		ClientClassID::CBasePlayer, xorstr_("DT_Local"), xorstr_("m_viewPunchAngle"))

	NETVAR_FUNCTION(int, ShotsFired, ClientClassID::CCSPlayer, xorstr_("DT_CSLocalPlayerExclusive"), xorstr_("m_iShotsFired"))
	NETVAR_FUNCTION(int, Health, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_iHealth"))

	NETVAR_FUNCTION(enum ObserverMode, ObserverMode, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_iObserverMode"))
	NETVAR_FUNCTION(void, ObserverTarget, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_hObserverTarget"))

	NETVAR_FUNCTION(unsigned int, TickBase, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_nTickBase"))

	NETVAR_FUNCTION(float, FlashMaxAlpha, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_flFlashMaxAlpha"))

	NETVAR_FUNCTION(Vector, Velocity, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecVelocity[0]"))
	NETVAR_FUNCTION(float, VelocityModifier, ClientClassID::CCSPlayer, xorstr_("DT_CSLocalPlayerExclusive"), xorstr_("m_flVelocityModifier"))

	NETVAR_FUNCTION(void, ViewModel, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_hViewModel[0]"))
	NETVAR_FUNCTION(bool, Scoped, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsScoped"))

	inline Vector GetEyePosition()
	{
		return *this->Origin() + *this->VecViewOffset();
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
