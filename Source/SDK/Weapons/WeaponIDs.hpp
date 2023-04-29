#ifndef SDK_WEAPONIDS
#define SDK_WEAPONIDS

#include "xorstr.hpp"

#include <cstring>
#include <map>

// Dumped using https://www.unknowncheats.me/forum/counterstrike-global-offensive/334782-weapon-id-dumper.html
// Porting it to linux is easy, download his code: https://github.com/DucaRii/csgo_weaponid_dumper
// Go into the csgo_weaponid_dumper.cpp and remove the first line (should be '#include <Windows.h>')
// Then compile using 'clang++ csgo_weaponid_dumper.cpp external/android_strings/android_strings.cpp'
// Move 'a.out' to ~/.steam/steam/steamapps/common/Counter-Strike Global Offensive/csgo/scripts/items
// and run it

// You may want to add WEAPON_INVALID (-1) and WEAPON_NONE (0) too

enum class WeaponID : short {
	WEAPON_INVALID = -1,
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_ZONE_REPULSOR = 20,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	STUDDED_BROKENFANG_GLOVES = 4725,
	STUDDED_BLOODHOUND_GLOVES = 5027,
	T_GLOVES = 5028,
	CT_GLOVES = 5029,
	SPORTY_GLOVES = 5030,
	SLICK_GLOVES = 5031,
	LEATHER_HANDWRAPS = 5032,
	MOTORCYCLE_GLOVES = 5033,
	SPECIALIST_GLOVES = 5034,
	STUDDED_HYDRA_GLOVES = 5035
};

static std::map<WeaponID, const char*> weaponLocalization {
	{ WeaponID::WEAPON_INVALID, strdup(xorstr_("Invalid weapon")) },
	{ WeaponID::WEAPON_NONE, strdup(xorstr_("None")) },
	{ WeaponID::WEAPON_DEAGLE, strdup(xorstr_("Desert Eagle")) },
	{ WeaponID::WEAPON_ELITE, strdup(xorstr_("Dual Berettas")) },
	{ WeaponID::WEAPON_FIVESEVEN, strdup(xorstr_("Five-SeveN")) },
	{ WeaponID::WEAPON_GLOCK, strdup(xorstr_("Glock-18")) },
	{ WeaponID::WEAPON_AK47, strdup(xorstr_("AK-47")) },
	{ WeaponID::WEAPON_AUG, strdup(xorstr_("AUG")) },
	{ WeaponID::WEAPON_AWP, strdup(xorstr_("AWP")) },
	{ WeaponID::WEAPON_FAMAS, strdup(xorstr_("FAMAS")) },
	{ WeaponID::WEAPON_G3SG1, strdup(xorstr_("G3SG1")) },
	{ WeaponID::WEAPON_GALILAR, strdup(xorstr_("Galil AR")) },
	{ WeaponID::WEAPON_M249, strdup(xorstr_("M249")) },
	{ WeaponID::WEAPON_M4A1, strdup(xorstr_("M4A4")) }, // Don't know why the game calls it M4A1 even though its M4A4
	{ WeaponID::WEAPON_MAC10, strdup(xorstr_("MAC-10")) },
	{ WeaponID::WEAPON_P90, strdup(xorstr_("P90")) },
	{ WeaponID::WEAPON_ZONE_REPULSOR, strdup(xorstr_("Zone Repulsor")) },
	{ WeaponID::WEAPON_MP5SD, strdup(xorstr_("MP5-SD")) },
	{ WeaponID::WEAPON_UMP45, strdup(xorstr_("UMP-45")) },
	{ WeaponID::WEAPON_XM1014, strdup(xorstr_("XM1014")) },
	{ WeaponID::WEAPON_BIZON, strdup(xorstr_("PP-Bizon")) },
	{ WeaponID::WEAPON_MAG7, strdup(xorstr_("MAG-7")) },
	{ WeaponID::WEAPON_NEGEV, strdup(xorstr_("Negev")) },
	{ WeaponID::WEAPON_SAWEDOFF, strdup(xorstr_("Sawed-Off")) },
	{ WeaponID::WEAPON_TEC9, strdup(xorstr_("Tec-9")) },
	{ WeaponID::WEAPON_TASER, strdup(xorstr_("Zeus x27")) },
	{ WeaponID::WEAPON_HKP2000, strdup(xorstr_("P2000")) },
	{ WeaponID::WEAPON_MP7, strdup(xorstr_("MP7")) },
	{ WeaponID::WEAPON_MP9, strdup(xorstr_("MP9")) },
	{ WeaponID::WEAPON_NOVA, strdup(xorstr_("Nova")) },
	{ WeaponID::WEAPON_P250, strdup(xorstr_("P250")) },
	{ WeaponID::WEAPON_SHIELD, strdup(xorstr_("Riot Shield")) },
	{ WeaponID::WEAPON_SCAR20, strdup(xorstr_("SCAR-20")) },
	{ WeaponID::WEAPON_SG556, strdup(xorstr_("SG 553")) }, // I don't know why the game calls it WEAPON_SG556, but it's not 556 but 553
	{ WeaponID::WEAPON_SSG08, strdup(xorstr_("SSG 08")) },
	{ WeaponID::WEAPON_KNIFEGG, strdup(xorstr_("Golden Knife")) },
	{ WeaponID::WEAPON_KNIFE, strdup(xorstr_("Knife")) },
	{ WeaponID::WEAPON_FLASHBANG, strdup(xorstr_("Flashbang")) },
	{ WeaponID::WEAPON_HEGRENADE, strdup(xorstr_("High Explosive Grenade")) },
	{ WeaponID::WEAPON_SMOKEGRENADE, strdup(xorstr_("Smoke Grenade")) },
	{ WeaponID::WEAPON_MOLOTOV, strdup(xorstr_("Molotov")) },
	{ WeaponID::WEAPON_DECOY, strdup(xorstr_("Decoy Grenade")) },
	{ WeaponID::WEAPON_INCGRENADE, strdup(xorstr_("Incendiary Grenade")) },
	{ WeaponID::WEAPON_C4, strdup(xorstr_("C4 Explosive")) },
	{ WeaponID::WEAPON_HEALTHSHOT, strdup(xorstr_("Medi-Shot")) },
	{ WeaponID::WEAPON_KNIFE_T, strdup(xorstr_("Knife")) },
	{ WeaponID::WEAPON_M4A1_SILENCER, strdup(xorstr_("M4A1-S")) },
	{ WeaponID::WEAPON_USP_SILENCER, strdup(xorstr_("USP-S")) },
	{ WeaponID::WEAPON_CZ75A, strdup(xorstr_("CZ75-Auto")) },
	{ WeaponID::WEAPON_REVOLVER, strdup(xorstr_("R8 Revolver")) },
	{ WeaponID::WEAPON_TAGRENADE, strdup(xorstr_("Tactical Awareness Grenade")) },
	{ WeaponID::WEAPON_FISTS, strdup(xorstr_("Bare Hands")) },
	{ WeaponID::WEAPON_BREACHCHARGE, strdup(xorstr_("Breach charge")) },
	{ WeaponID::WEAPON_TABLET, strdup(xorstr_("Tablet")) },
	{ WeaponID::WEAPON_MELEE, strdup(xorstr_("Knife")) }, // A broken and throwable variant...
	{ WeaponID::WEAPON_AXE, strdup(xorstr_("Axe")) },
	{ WeaponID::WEAPON_HAMMER, strdup(xorstr_("Hammer")) },
	{ WeaponID::WEAPON_SPANNER, strdup(xorstr_("Wrench")) },
	{ WeaponID::WEAPON_KNIFE_GHOST, strdup(xorstr_("Spectral Shiv")) }, // Shoutout to whatever valve employee who is responsible for this one, it's awesome
	{ WeaponID::WEAPON_FIREBOMB, strdup(xorstr_("Fire Bomb")) }, // Literally the same as a molotov?
	{ WeaponID::WEAPON_DIVERSION, strdup(xorstr_("Diversion")) }, // Decoy, but footsteps instead of gunshots? (lmao that's cool, why don't we have that in comp)
	{ WeaponID::WEAPON_FRAG_GRENADE, strdup(xorstr_("Frag Grenade")) },
	{ WeaponID::WEAPON_SNOWBALL, strdup(xorstr_("Snowball")) },
	{ WeaponID::WEAPON_BUMPMINE, strdup(xorstr_("Bump Mine")) },
	{ WeaponID::WEAPON_BAYONET, strdup(xorstr_("Bayonet")) },
	{ WeaponID::WEAPON_KNIFE_CSS, strdup(xorstr_("Classic Knife")) },
	{ WeaponID::WEAPON_KNIFE_FLIP, strdup(xorstr_("Flip Knife")) },
	{ WeaponID::WEAPON_KNIFE_GUT, strdup(xorstr_("Gut Knife")) },
	{ WeaponID::WEAPON_KNIFE_KARAMBIT, strdup(xorstr_("Karambit")) },
	{ WeaponID::WEAPON_KNIFE_M9_BAYONET, strdup(xorstr_("M9 Bayonet")) },
	{ WeaponID::WEAPON_KNIFE_TACTICAL, strdup(xorstr_("Huntsman Knife")) },
	{ WeaponID::WEAPON_KNIFE_FALCHION, strdup(xorstr_("Falchion Knife")) },
	{ WeaponID::WEAPON_KNIFE_SURVIVAL_BOWIE, strdup(xorstr_("Bowie Knife")) },
	{ WeaponID::WEAPON_KNIFE_BUTTERFLY, strdup(xorstr_("Butterfly Knife")) },
	{ WeaponID::WEAPON_KNIFE_PUSH, strdup(xorstr_("Shadow daggers")) },
	{ WeaponID::WEAPON_KNIFE_CORD, strdup(xorstr_("Paracord Knife")) },
	{ WeaponID::WEAPON_KNIFE_CANIS, strdup(xorstr_("Survival Knife")) },
	{ WeaponID::WEAPON_KNIFE_URSUS, strdup(xorstr_("Ursus Knife")) },
	{ WeaponID::WEAPON_KNIFE_GYPSY_JACKKNIFE, strdup(xorstr_("Navaja Knife")) },
	{ WeaponID::WEAPON_KNIFE_OUTDOOR, strdup(xorstr_("Nomad Knife")) },
	{ WeaponID::WEAPON_KNIFE_STILETTO, strdup(xorstr_("Stiletto Knife")) },
	{ WeaponID::WEAPON_KNIFE_WIDOWMAKER, strdup(xorstr_("Talon Knife")) },
	{ WeaponID::WEAPON_KNIFE_SKELETON, strdup(xorstr_("Skeleton Knife")) },
	{ WeaponID::STUDDED_BROKENFANG_GLOVES, strdup(xorstr_("Broken Fang Gloves")) },
	{ WeaponID::STUDDED_BLOODHOUND_GLOVES, strdup(xorstr_("Bloodhound Gloves")) },
	{ WeaponID::T_GLOVES, strdup(xorstr_("T Gloves")) },
	{ WeaponID::CT_GLOVES, strdup(xorstr_("CT Gloves")) },
	{ WeaponID::SPORTY_GLOVES, strdup(xorstr_("Sport Gloves")) },
	{ WeaponID::SLICK_GLOVES, strdup(xorstr_("Slick Gloves")) }, // Seem to be unused content (or go by different name)
	{ WeaponID::LEATHER_HANDWRAPS, strdup(xorstr_("Hand Wraps")) },
	{ WeaponID::MOTORCYCLE_GLOVES, strdup(xorstr_("Motorcycle Gloves")) }, // Seem to be unused content (or go by different name)
	{ WeaponID::SPECIALIST_GLOVES, strdup(xorstr_("Specialist Gloves")) },
	{ WeaponID::STUDDED_HYDRA_GLOVES, strdup(xorstr_("Hydra Gloves")) }
};

inline const char* LocalizeWeaponID(WeaponID weaponID)
{
	if (!weaponLocalization.contains(weaponID))
		return nullptr;
	return weaponLocalization[weaponID];
}

inline bool IsFirearm(WeaponID weaponID)
{
	return ((weaponID >= WeaponID::WEAPON_DEAGLE && weaponID <= WeaponID::WEAPON_SSG08) || (weaponID >= WeaponID::WEAPON_M4A1_SILENCER && weaponID <= WeaponID::WEAPON_REVOLVER)) && weaponID != WeaponID::WEAPON_TASER /* Technically considered a firearm, but it behaves fundamentally differently */ && weaponID != WeaponID::WEAPON_ZONE_REPULSOR;
}

inline bool IsKnife(WeaponID weaponID)
{
	return (weaponID >= WeaponID::WEAPON_KNIFEGG && weaponID <= WeaponID::WEAPON_KNIFE) || weaponID == WeaponID::WEAPON_KNIFE_T || weaponID == WeaponID::WEAPON_KNIFE_GHOST || (weaponID >= WeaponID::WEAPON_KNIFE_CSS && weaponID <= WeaponID::WEAPON_KNIFE_SKELETON);
}

#endif