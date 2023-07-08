#ifndef SDK_WEAPONS_WEAPONIDS
#define SDK_WEAPONS_WEAPONIDS

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

static std::map<WeaponID, const char*> weaponLocalization{
	{ WeaponID::WEAPON_INVALID, "Invalid weapon" },
	{ WeaponID::WEAPON_NONE, "None" },
	{ WeaponID::WEAPON_DEAGLE, "Desert Eagle" },
	{ WeaponID::WEAPON_ELITE, "Dual Berettas" },
	{ WeaponID::WEAPON_FIVESEVEN, "Five-SeveN" },
	{ WeaponID::WEAPON_GLOCK, "Glock-18" },
	{ WeaponID::WEAPON_AK47, "AK-47" },
	{ WeaponID::WEAPON_AUG, "AUG" },
	{ WeaponID::WEAPON_AWP, "AWP" },
	{ WeaponID::WEAPON_FAMAS, "FAMAS" },
	{ WeaponID::WEAPON_G3SG1, "G3SG1" },
	{ WeaponID::WEAPON_GALILAR, "Galil AR" },
	{ WeaponID::WEAPON_M249, "M249" },
	{ WeaponID::WEAPON_M4A1, "M4A4" }, // Don't know why the game calls it M4A1 even though its M4A4
	{ WeaponID::WEAPON_MAC10, "MAC-10" },
	{ WeaponID::WEAPON_P90, "P90" },
	{ WeaponID::WEAPON_ZONE_REPULSOR, "Zone Repulsor" },
	{ WeaponID::WEAPON_MP5SD, "MP5-SD" },
	{ WeaponID::WEAPON_UMP45, "UMP-45" },
	{ WeaponID::WEAPON_XM1014, "XM1014" },
	{ WeaponID::WEAPON_BIZON, "PP-Bizon" },
	{ WeaponID::WEAPON_MAG7, "MAG-7" },
	{ WeaponID::WEAPON_NEGEV, "Negev" },
	{ WeaponID::WEAPON_SAWEDOFF, "Sawed-Off" },
	{ WeaponID::WEAPON_TEC9, "Tec-9" },
	{ WeaponID::WEAPON_TASER, "Zeus x27" },
	{ WeaponID::WEAPON_HKP2000, "P2000" },
	{ WeaponID::WEAPON_MP7, "MP7" },
	{ WeaponID::WEAPON_MP9, "MP9" },
	{ WeaponID::WEAPON_NOVA, "Nova" },
	{ WeaponID::WEAPON_P250, "P250" },
	{ WeaponID::WEAPON_SHIELD, "Riot Shield" },
	{ WeaponID::WEAPON_SCAR20, "SCAR-20" },
	{ WeaponID::WEAPON_SG556, "SG 553" }, // I don't know why the game calls it WEAPON_SG556, but it's not 556 but 553
	{ WeaponID::WEAPON_SSG08, "SSG 08" },
	{ WeaponID::WEAPON_KNIFEGG, "Golden Knife" },
	{ WeaponID::WEAPON_KNIFE, "Knife" },
	{ WeaponID::WEAPON_FLASHBANG, "Flashbang" },
	{ WeaponID::WEAPON_HEGRENADE, "High Explosive Grenade" },
	{ WeaponID::WEAPON_SMOKEGRENADE, "Smoke Grenade" },
	{ WeaponID::WEAPON_MOLOTOV, "Molotov" },
	{ WeaponID::WEAPON_DECOY, "Decoy Grenade" },
	{ WeaponID::WEAPON_INCGRENADE, "Incendiary Grenade" },
	{ WeaponID::WEAPON_C4, "C4 Explosive" },
	{ WeaponID::WEAPON_HEALTHSHOT, "Medi-Shot" },
	{ WeaponID::WEAPON_KNIFE_T, "Knife" },
	{ WeaponID::WEAPON_M4A1_SILENCER, "M4A1-S" },
	{ WeaponID::WEAPON_USP_SILENCER, "USP-S" },
	{ WeaponID::WEAPON_CZ75A, "CZ75-Auto" },
	{ WeaponID::WEAPON_REVOLVER, "R8 Revolver" },
	{ WeaponID::WEAPON_TAGRENADE, "Tactical Awareness Grenade" },
	{ WeaponID::WEAPON_FISTS, "Bare Hands" },
	{ WeaponID::WEAPON_BREACHCHARGE, "Breach charge" },
	{ WeaponID::WEAPON_TABLET, "Tablet" },
	{ WeaponID::WEAPON_MELEE, "Knife" }, // A broken and throwable variant...
	{ WeaponID::WEAPON_AXE, "Axe" },
	{ WeaponID::WEAPON_HAMMER, "Hammer" },
	{ WeaponID::WEAPON_SPANNER, "Wrench" },
	{ WeaponID::WEAPON_KNIFE_GHOST, "Spectral Shiv" }, // Shoutout to whatever valve employee who is responsible for this one, it's awesome
	{ WeaponID::WEAPON_FIREBOMB, "Fire Bomb" }, // Literally the same as a molotov?
	{ WeaponID::WEAPON_DIVERSION, "Diversion" }, // Decoy, but footsteps instead of gunshots? (lmao that's cool, why don't we have that in comp)
	{ WeaponID::WEAPON_FRAG_GRENADE, "Frag Grenade" },
	{ WeaponID::WEAPON_SNOWBALL, "Snowball" },
	{ WeaponID::WEAPON_BUMPMINE, "Bump Mine" },
	{ WeaponID::WEAPON_BAYONET, "Bayonet" },
	{ WeaponID::WEAPON_KNIFE_CSS, "Classic Knife" },
	{ WeaponID::WEAPON_KNIFE_FLIP, "Flip Knife" },
	{ WeaponID::WEAPON_KNIFE_GUT, "Gut Knife" },
	{ WeaponID::WEAPON_KNIFE_KARAMBIT, "Karambit" },
	{ WeaponID::WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
	{ WeaponID::WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
	{ WeaponID::WEAPON_KNIFE_FALCHION, "Falchion Knife" },
	{ WeaponID::WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife" },
	{ WeaponID::WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
	{ WeaponID::WEAPON_KNIFE_PUSH, "Shadow daggers" },
	{ WeaponID::WEAPON_KNIFE_CORD, "Paracord Knife" },
	{ WeaponID::WEAPON_KNIFE_CANIS, "Survival Knife" },
	{ WeaponID::WEAPON_KNIFE_URSUS, "Ursus Knife" },
	{ WeaponID::WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife" },
	{ WeaponID::WEAPON_KNIFE_OUTDOOR, "Nomad Knife" },
	{ WeaponID::WEAPON_KNIFE_STILETTO, "Stiletto Knife" },
	{ WeaponID::WEAPON_KNIFE_WIDOWMAKER, "Talon Knife" },
	{ WeaponID::WEAPON_KNIFE_SKELETON, "Skeleton Knife" },
	{ WeaponID::STUDDED_BROKENFANG_GLOVES, "Broken Fang Gloves" },
	{ WeaponID::STUDDED_BLOODHOUND_GLOVES, "Bloodhound Gloves" },
	{ WeaponID::T_GLOVES, "T Gloves" },
	{ WeaponID::CT_GLOVES, "CT Gloves" },
	{ WeaponID::SPORTY_GLOVES, "Sport Gloves" },
	{ WeaponID::SLICK_GLOVES, "Slick Gloves" }, // Seem to be unused content (or go by different name)
	{ WeaponID::LEATHER_HANDWRAPS, "Hand Wraps" },
	{ WeaponID::MOTORCYCLE_GLOVES, "Motorcycle Gloves" }, // Seem to be unused content (or go by different name)
	{ WeaponID::SPECIALIST_GLOVES, "Specialist Gloves" },
	{ WeaponID::STUDDED_HYDRA_GLOVES, "Hydra Gloves" }
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

inline bool IsGrenade(WeaponID weaponID)
{
	return (weaponID >= WeaponID::WEAPON_FLASHBANG && weaponID <= WeaponID::WEAPON_INCGRENADE) || weaponID == WeaponID::WEAPON_TAGRENADE || weaponID == WeaponID::WEAPON_BREACHCHARGE || (weaponID >= WeaponID::WEAPON_FIREBOMB && weaponID <= WeaponID::WEAPON_SNOWBALL);
}

#endif