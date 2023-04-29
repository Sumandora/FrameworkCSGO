#ifndef UTILS_WEAPONCONFIG_WEAPONCLASSES
#define UTILS_WEAPONCONFIG_WEAPONCLASSES

#include "xorstr.hpp"

#include "../../SDK/Weapons/WeaponIDs.hpp"

#include <map>
#include <vector>

enum class WeaponClass {
	// This doesn't exist in source engine.
	NONE,

	PISTOL,
	HEAVY_PISTOL,

	SMG,

	RIFLE,

	SNIPER,
	AUTO_SNIPER,

	MACHINE_GUN,
	SHOTGUN
};

static std::map<WeaponClass, const char*> weaponClassLocalization {
	{ WeaponClass::NONE, strdup(xorstr_("None")) },

	{ WeaponClass::PISTOL, strdup(xorstr_("Pistol")) },
	{ WeaponClass::HEAVY_PISTOL, strdup(xorstr_("Heavy pistol")) },

	{ WeaponClass::SMG, strdup(xorstr_("SMG")) },

	{ WeaponClass::RIFLE, strdup(xorstr_("Rifle")) },

	{ WeaponClass::SNIPER, strdup(xorstr_("Sniper")) },
	{ WeaponClass::AUTO_SNIPER, strdup(xorstr_("Auto sniper")) },

	{ WeaponClass::MACHINE_GUN, strdup(xorstr_("Machine gun")) },
	{ WeaponClass::SHOTGUN, strdup(xorstr_("Shotgun")) }
};

static std::map<WeaponClass, std::vector<WeaponID>> weaponClassification {
	{ WeaponClass::PISTOL, { WeaponID::WEAPON_HKP2000, WeaponID::WEAPON_USP_SILENCER, WeaponID::WEAPON_GLOCK, WeaponID::WEAPON_ELITE, WeaponID::WEAPON_P250, WeaponID::WEAPON_CZ75A, WeaponID::WEAPON_FIVESEVEN, WeaponID::WEAPON_TEC9 } },
	{ WeaponClass::HEAVY_PISTOL, { WeaponID::WEAPON_DEAGLE, WeaponID::WEAPON_REVOLVER } },
	{ WeaponClass::SMG, { WeaponID::WEAPON_MP9, WeaponID::WEAPON_MAC10, WeaponID::WEAPON_MP5SD, WeaponID::WEAPON_MP7, WeaponID::WEAPON_UMP45, WeaponID::WEAPON_P90, WeaponID::WEAPON_BIZON } },
	{ WeaponClass::RIFLE, { WeaponID::WEAPON_FAMAS, WeaponID::WEAPON_GALILAR, WeaponID::WEAPON_M4A1_SILENCER, WeaponID::WEAPON_M4A1, WeaponID::WEAPON_AK47, WeaponID::WEAPON_AUG, WeaponID::WEAPON_SG556 } },
	{ WeaponClass::SNIPER, { WeaponID::WEAPON_SSG08, WeaponID::WEAPON_AWP } },
	{ WeaponClass::AUTO_SNIPER, { WeaponID::WEAPON_SCAR20, WeaponID::WEAPON_G3SG1 } },
	{ WeaponClass::MACHINE_GUN, { WeaponID::WEAPON_M249, WeaponID::WEAPON_NEGEV } },
	{ WeaponClass::SHOTGUN, { WeaponID::WEAPON_NOVA, WeaponID::WEAPON_XM1014, WeaponID::WEAPON_MAG7, WeaponID::WEAPON_SAWEDOFF } }
};

inline WeaponClass WeaponClassByID(WeaponID weaponId)
{
	for (auto [weaponClass, weaponIds] : weaponClassification) {
		for (const WeaponID weaponId2 : weaponIds) {
			if (weaponId == weaponId2)
				return weaponClass;
		}
	}
	return WeaponClass::NONE;
}

#endif