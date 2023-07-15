#ifndef SDK_GAMECLASS_ENTITIES_CBASEPLAYER
#define SDK_GAMECLASS_ENTITIES_CBASEPLAYER

#include "CBaseCombatWeapon.hpp"
#include "CBaseEntity.hpp"

#include "../../Definitions/LifeState.hpp"
#include "../../ObserverMode.hpp"

#include "../../../ConVarStorage.hpp"

#include "../../CBaseHandle.hpp"
#include "../../Math/Vector.hpp"

#define MAX_PLACE_NAME_LENGTH 18
#define MAX_WEAPONS 64

class CBasePlayer : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Flags, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_fFlags")
	NETVAR_FUNCTION(char, LifeState, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_lifeState")
	NETVAR_FUNCTION(bool, GunGameImmunity, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bGunGameImmunity")
	NETVAR_FUNCTION(Vector, VecViewOffset, ClientClassID::CBasePlayer, "DT_LocalPlayerExclusive", "m_vecViewOffset[0]")
	NETVAR_FUNCTION(bool, SurvivalTeam, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_nSurvivalTeam")

	NETVAR_FUNCTION(CBaseHandle, ActiveWeapon, ClientClassID::CBaseCombatCharacter, "DT_BaseCombatCharacter", "m_hActiveWeapon")
	NETVAR_FUNCTION(CBaseHandle, Weapons, ClientClassID::CBaseCombatCharacter, "DT_BaseCombatCharacter", "m_hMyWeapons")
	NETVAR_FUNCTION(bool, WaitForNoAttack, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bWaitForNoAttack")

	COMBINED_NETVAR_FUNCTION(Vector, AimPunchAngle,
		ClientClassID::CBasePlayer, "DT_LocalPlayerExclusive", "m_Local",
		ClientClassID::CBasePlayer, "DT_Local", "m_aimPunchAngle")
	COMBINED_NETVAR_FUNCTION(Vector, ViewPunchAngle,
		ClientClassID::CBasePlayer, "DT_LocalPlayerExclusive", "m_Local",
		ClientClassID::CBasePlayer, "DT_Local", "m_viewPunchAngle")

	NETVAR_FUNCTION(int, ShotsFired, ClientClassID::CCSPlayer, "DT_CSLocalPlayerExclusive", "m_iShotsFired")
	NETVAR_FUNCTION(int, Health, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_iHealth")

	NETVAR_FUNCTION(enum ObserverMode, ObserverMode, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_iObserverMode")
	NETVAR_FUNCTION(CBaseHandle, ObserverTarget, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_hObserverTarget")

	NETVAR_FUNCTION(unsigned int, TickBase, ClientClassID::CBasePlayer, "DT_LocalPlayerExclusive", "m_nTickBase")

	NETVAR_FUNCTION(float, FlashMaxAlpha, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_flFlashMaxAlpha")
	inline float GetFlashAlpha()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<char*>(FlashMaxAlpha()) - 0x8);
	}

	NETVAR_FUNCTION(Vector, Velocity, ClientClassID::CBasePlayer, "DT_LocalPlayerExclusive", "m_vecVelocity[0]")
	NETVAR_FUNCTION(float, VelocityModifier, ClientClassID::CCSPlayer, "DT_CSLocalPlayerExclusive", "m_flVelocityModifier")

	NETVAR_FUNCTION(CBaseHandle, ViewModel, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_hViewModel[0]")
	NETVAR_FUNCTION(bool, Scoped, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bIsScoped")
	NETVAR_FUNCTION(int, Account, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_iAccount")
	NETVAR_FUNCTION(char, LastPlaceName, ClientClassID::CBasePlayer, "DT_BasePlayer", "m_szLastPlaceName")

	NETVAR_FUNCTION(bool, IsDefusing, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bIsDefusing")
	NETVAR_FUNCTION(bool, IsGrabbingHostage, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bIsGrabbingHostage")
	NETVAR_FUNCTION(bool, IsRescuing, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bIsRescuing")

	NETVAR_FUNCTION(bool, HasDefuser, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bHasDefuser")

	NETVAR_FUNCTION(bool, IsWalking, ClientClassID::CCSPlayer, "DT_CSPlayer", "m_bIsWalking")

	inline Vector GetEyePosition()
	{
		return *this->Origin() + *this->VecViewOffset();
	}

	inline bool IsAlive()
	{
		return *LifeState() == LIFE_ALIVE && *Health() > 0;
	}

	inline bool IsEnemy(CBasePlayer* view)
	{
		if (ConVarStorage::mp_teammates_are_enemies()->GetBool())
			return true;
		if (ConVarStorage::game_type()->GetInt() == 6) {
			const int viewerSurvivalTeam = *view->SurvivalTeam();
			if (viewerSurvivalTeam < 0) // DZ without teams
				return true;

			return viewerSurvivalTeam != *SurvivalTeam();
		}
		return *view->Team() != *Team();
	}
};

#endif
