#ifndef SDK_GAMECLASS_CBASEPLAYER
#define SDK_GAMECLASS_CBASEPLAYER

#include "CBaseCombatWeapon.hpp"
#include "CBaseEntity.hpp"

#include "../Definitions/LifeState.hpp"
#include "../ObserverMode.hpp"

#include "../../ConVarStorage.hpp"

#include "../CBaseHandle.hpp"
#include "../Math/Vector.hpp"

#define MAX_PLACE_NAME_LENGTH 18
#define MAX_WEAPONS 64

class CBasePlayer : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Flags, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_fFlags"))
	NETVAR_FUNCTION(char, LifeState, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_lifeState"))
	NETVAR_FUNCTION(bool, GunGameImmunity, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bGunGameImmunity"))
	NETVAR_FUNCTION(Vector, VecViewOffset, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecViewOffset[0]"))
	NETVAR_FUNCTION(bool, SurvivalTeam, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_nSurvivalTeam"))

	NETVAR_FUNCTION(CBaseHandle, ActiveWeapon, ClientClassID::CBaseCombatCharacter, xorstr_("DT_BaseCombatCharacter"), xorstr_("m_hActiveWeapon"))
	NETVAR_FUNCTION(CBaseHandle, Weapons, ClientClassID::CBaseCombatCharacter, xorstr_("DT_BaseCombatCharacter"), xorstr_("m_hMyWeapons"))
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
	NETVAR_FUNCTION(CBaseHandle, ObserverTarget, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_hObserverTarget"))

	NETVAR_FUNCTION(unsigned int, TickBase, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_nTickBase"))

	NETVAR_FUNCTION(float, FlashMaxAlpha, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_flFlashMaxAlpha"))
	inline float GetFlashAlpha()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<char*>(FlashMaxAlpha()) - 0x8);
	}

	NETVAR_FUNCTION(Vector, Velocity, ClientClassID::CBasePlayer, xorstr_("DT_LocalPlayerExclusive"), xorstr_("m_vecVelocity[0]"))
	NETVAR_FUNCTION(float, VelocityModifier, ClientClassID::CCSPlayer, xorstr_("DT_CSLocalPlayerExclusive"), xorstr_("m_flVelocityModifier"))

	NETVAR_FUNCTION(CBaseHandle, ViewModel, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_hViewModel[0]"))
	NETVAR_FUNCTION(bool, Scoped, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsScoped"))
	NETVAR_FUNCTION(int, Account, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_iAccount"))
	NETVAR_FUNCTION(char, LastPlaceName, ClientClassID::CBasePlayer, xorstr_("DT_BasePlayer"), xorstr_("m_szLastPlaceName"))

	NETVAR_FUNCTION(bool, IsDefusing, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsDefusing"))
	NETVAR_FUNCTION(bool, IsGrabbingHostage, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsGrabbingHostage"))
	NETVAR_FUNCTION(bool, IsRescuing, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsRescuing"))

	NETVAR_FUNCTION(bool, HasDefuser, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bHasDefuser"))

	NETVAR_FUNCTION(bool, IsWalking, ClientClassID::CCSPlayer, xorstr_("DT_CSPlayer"), xorstr_("m_bIsWalking"))

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
