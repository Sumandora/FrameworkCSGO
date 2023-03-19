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
	STUDDED_HYDRA_GLOVES = 5035,
};

// I need to make huge functions like this one, because the xorstr lib produces dangling pointers otherwise
inline constexpr void LocalizeWeaponID(WeaponID weaponID, char weaponName[256])
{
	switch (weaponID) {
	case WeaponID::WEAPON_INVALID:
		strcpy(weaponName, xorstr_("Invalid weapon"));
		break;
	case WeaponID::WEAPON_NONE:
		strcpy(weaponName, xorstr_("None"));
		break;
	case WeaponID::WEAPON_DEAGLE:
		strcpy(weaponName, xorstr_("Desert Eagle"));
		break;
	case WeaponID::WEAPON_ELITE:
		strcpy(weaponName, xorstr_("Dual Berettas"));
		break;
	case WeaponID::WEAPON_FIVESEVEN:
		strcpy(weaponName, xorstr_("Five-SeveN"));
		break;
	case WeaponID::WEAPON_GLOCK:
		strcpy(weaponName, xorstr_("Glock-18"));
		break;
	case WeaponID::WEAPON_AK47:
		strcpy(weaponName, xorstr_("AK-47"));
		break;
	case WeaponID::WEAPON_AUG:
		strcpy(weaponName, xorstr_("AUG"));
		break;
	case WeaponID::WEAPON_AWP:
		strcpy(weaponName, xorstr_("AWP"));
		break;
	case WeaponID::WEAPON_FAMAS:
		strcpy(weaponName, xorstr_("FAMAS"));
		break;
	case WeaponID::WEAPON_G3SG1:
		strcpy(weaponName, xorstr_("G3SG1"));
		break;
	case WeaponID::WEAPON_GALILAR:
		strcpy(weaponName, xorstr_("Galil AR"));
		break;
	case WeaponID::WEAPON_M249:
		strcpy(weaponName, xorstr_("M249"));
		break;
	case WeaponID::WEAPON_M4A1:
		strcpy(weaponName, xorstr_("M4A4"));
		break; // Don't know why the game calls it M4A1 even though the its M4A4
	case WeaponID::WEAPON_MAC10:
		strcpy(weaponName, xorstr_("MAC-10"));
		break;
	case WeaponID::WEAPON_P90:
		strcpy(weaponName, xorstr_("P90"));
		break;
	case WeaponID::WEAPON_ZONE_REPULSOR:
		strcpy(weaponName, xorstr_("Zone Repulsor"));
		break;
	case WeaponID::WEAPON_MP5SD:
		strcpy(weaponName, xorstr_("MP5-SD"));
		break;
	case WeaponID::WEAPON_UMP45:
		strcpy(weaponName, xorstr_("UMP-45"));
		break;
	case WeaponID::WEAPON_XM1014:
		strcpy(weaponName, xorstr_("XM1014"));
		break;
	case WeaponID::WEAPON_BIZON:
		strcpy(weaponName, xorstr_("PP-Bizon"));
		break;
	case WeaponID::WEAPON_MAG7:
		strcpy(weaponName, xorstr_("MAG-7"));
		break;
	case WeaponID::WEAPON_NEGEV:
		strcpy(weaponName, xorstr_("Negev"));
		break;
	case WeaponID::WEAPON_SAWEDOFF:
		strcpy(weaponName, xorstr_("Sawed-Off"));
		break;
	case WeaponID::WEAPON_TEC9:
		strcpy(weaponName, xorstr_("Tec-9"));
		break;
	case WeaponID::WEAPON_TASER:
		strcpy(weaponName, xorstr_("Zeus x27"));
		break;
	case WeaponID::WEAPON_HKP2000:
		strcpy(weaponName, xorstr_("P2000"));
		break;
	case WeaponID::WEAPON_MP7:
		strcpy(weaponName, xorstr_("MP7"));
		break;
	case WeaponID::WEAPON_MP9:
		strcpy(weaponName, xorstr_("MP9"));
		break;
	case WeaponID::WEAPON_NOVA:
		strcpy(weaponName, xorstr_("Nova"));
		break;
	case WeaponID::WEAPON_P250:
		strcpy(weaponName, xorstr_("P250"));
		break;
	case WeaponID::WEAPON_SHIELD:
		strcpy(weaponName, xorstr_("Riot Shield"));
		break;
	case WeaponID::WEAPON_SCAR20:
		strcpy(weaponName, xorstr_("SCAR-20"));
		break;
	case WeaponID::WEAPON_SG556:
		strcpy(weaponName, xorstr_("SG 553"));
		break; // I don't know why the game calls it WEAPON_SG556 but its not 556 but 553
	case WeaponID::WEAPON_SSG08:
		strcpy(weaponName, xorstr_("SSG 08"));
		break;
	case WeaponID::WEAPON_KNIFEGG:
		strcpy(weaponName, xorstr_("Golden Knife"));
		break;
	case WeaponID::WEAPON_KNIFE:
		strcpy(weaponName, xorstr_("Knife"));
		break;
	case WeaponID::WEAPON_FLASHBANG:
		strcpy(weaponName, xorstr_("Flashbang"));
		break;
	case WeaponID::WEAPON_HEGRENADE:
		strcpy(weaponName, xorstr_("High Explosive Grenade"));
		break;
	case WeaponID::WEAPON_SMOKEGRENADE:
		strcpy(weaponName, xorstr_("Smoke Grenade"));
		break;
	case WeaponID::WEAPON_MOLOTOV:
		strcpy(weaponName, xorstr_("Molotov"));
		break;
	case WeaponID::WEAPON_DECOY:
		strcpy(weaponName, xorstr_("Decoy Grenade"));
		break;
	case WeaponID::WEAPON_INCGRENADE:
		strcpy(weaponName, xorstr_("Incendiary Grenade"));
		break;
	case WeaponID::WEAPON_C4:
		strcpy(weaponName, xorstr_("C4 Explosive"));
		break;
	case WeaponID::WEAPON_HEALTHSHOT:
		strcpy(weaponName, xorstr_("Medi-Shot"));
		break;
	case WeaponID::WEAPON_KNIFE_T:
		strcpy(weaponName, xorstr_("Knife"));
		break;
	case WeaponID::WEAPON_M4A1_SILENCER:
		strcpy(weaponName, xorstr_("M4A1-S"));
		break;
	case WeaponID::WEAPON_USP_SILENCER:
		strcpy(weaponName, xorstr_("USP-S"));
		break;
	case WeaponID::WEAPON_CZ75A:
		strcpy(weaponName, xorstr_("CZ75-Auto"));
		break;
	case WeaponID::WEAPON_REVOLVER:
		strcpy(weaponName, xorstr_("R8 Revolver"));
		break;
	case WeaponID::WEAPON_TAGRENADE:
		strcpy(weaponName, xorstr_("Tactical Awareness Grenade"));
		break;
	case WeaponID::WEAPON_FISTS:
		strcpy(weaponName, xorstr_("Bare Hands"));
		break;
	case WeaponID::WEAPON_BREACHCHARGE:
		strcpy(weaponName, xorstr_("Breach charge"));
		break;
	case WeaponID::WEAPON_TABLET:
		strcpy(weaponName, xorstr_("Tablet"));
		break;
	case WeaponID::WEAPON_MELEE:
		strcpy(weaponName, xorstr_("Knife"));
		break; // A broken and throwable variant...
	case WeaponID::WEAPON_AXE:
		strcpy(weaponName, xorstr_("Axe"));
		break;
	case WeaponID::WEAPON_HAMMER:
		strcpy(weaponName, xorstr_("Hammer"));
		break;
	case WeaponID::WEAPON_SPANNER:
		strcpy(weaponName, xorstr_("Wrench"));
		break;
	case WeaponID::WEAPON_KNIFE_GHOST:
		strcpy(weaponName, xorstr_("Spectral Shiv"));
		break; // Shoutout to whatever valve employee is responsible for this one, it's awesome
	case WeaponID::WEAPON_FIREBOMB:
		strcpy(weaponName, xorstr_("Fire Bomb"));
		break; // Literally the same as a molotov?
	case WeaponID::WEAPON_DIVERSION:
		strcpy(weaponName, xorstr_("Diversion"));
		break; // Decoy, but footsteps instead of gunshots? (lmao thats cool, why don't we have that in comp)
	case WeaponID::WEAPON_FRAG_GRENADE:
		strcpy(weaponName, xorstr_("Frag Grenade"));
		break;
	case WeaponID::WEAPON_SNOWBALL:
		strcpy(weaponName, xorstr_("Snowball"));
		break;
	case WeaponID::WEAPON_BUMPMINE:
		strcpy(weaponName, xorstr_("Bump Mine"));
		break;
	case WeaponID::WEAPON_BAYONET:
		strcpy(weaponName, xorstr_("Bayonet"));
		break;
	case WeaponID::WEAPON_KNIFE_CSS:
		strcpy(weaponName, xorstr_("Classic Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_FLIP:
		strcpy(weaponName, xorstr_("Flip Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_GUT:
		strcpy(weaponName, xorstr_("Gut Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_KARAMBIT:
		strcpy(weaponName, xorstr_("Karambit"));
		break;
	case WeaponID::WEAPON_KNIFE_M9_BAYONET:
		strcpy(weaponName, xorstr_("M9 Bayonet"));
		break;
	case WeaponID::WEAPON_KNIFE_TACTICAL:
		strcpy(weaponName, xorstr_("Huntsman Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_FALCHION:
		strcpy(weaponName, xorstr_("Falchion Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_SURVIVAL_BOWIE:
		strcpy(weaponName, xorstr_("Bowie Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_BUTTERFLY:
		strcpy(weaponName, xorstr_("Butterfly Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_PUSH:
		strcpy(weaponName, xorstr_("Shadow daggers"));
		break;
	case WeaponID::WEAPON_KNIFE_CORD:
		strcpy(weaponName, xorstr_("Paracord Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_CANIS:
		strcpy(weaponName, xorstr_("Survival Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_URSUS:
		strcpy(weaponName, xorstr_("Ursus Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_GYPSY_JACKKNIFE:
		strcpy(weaponName, xorstr_("Navaja Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_OUTDOOR:
		strcpy(weaponName, xorstr_("Nomad Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_STILETTO:
		strcpy(weaponName, xorstr_("Stiletto Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_WIDOWMAKER:
		strcpy(weaponName, xorstr_("Talon Knife"));
		break;
	case WeaponID::WEAPON_KNIFE_SKELETON:
		strcpy(weaponName, xorstr_("Skeleton Knife"));
		break;
	case WeaponID::STUDDED_BROKENFANG_GLOVES:
		strcpy(weaponName, xorstr_("Broken Fang Gloves"));
		break;
	case WeaponID::STUDDED_BLOODHOUND_GLOVES:
		strcpy(weaponName, xorstr_("Bloodhound Gloves"));
		break;
	case WeaponID::T_GLOVES:
		strcpy(weaponName, xorstr_("T Gloves"));
		break;
	case WeaponID::CT_GLOVES:
		strcpy(weaponName, xorstr_("CT Gloves"));
		break;
	case WeaponID::SPORTY_GLOVES:
		strcpy(weaponName, xorstr_("Sport Gloves"));
		break;
	case WeaponID::SLICK_GLOVES:
		strcpy(weaponName, xorstr_("Slick Gloves"));
		break; // Seem to be unused content (or go by different name)
	case WeaponID::LEATHER_HANDWRAPS:
		strcpy(weaponName, xorstr_("Hand Wraps"));
		break;
	case WeaponID::MOTORCYCLE_GLOVES:
		strcpy(weaponName, xorstr_("Motorcycle Gloves"));
		break; // Seem to be unused content (or go by different name)
	case WeaponID::SPECIALIST_GLOVES:
		strcpy(weaponName, xorstr_("Specialist Gloves"));
		break;
	case WeaponID::STUDDED_HYDRA_GLOVES:
		strcpy(weaponName, xorstr_("Hydra Gloves"));
		break;
	default:
		strcpy(weaponName, xorstr_("Invalid weapon ID"));
		break;
	}
}

#endif