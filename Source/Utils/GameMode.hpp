#ifndef UTILS_GAMEMODE
#define UTILS_GAMEMODE

// This represents a gamemode, csgo splits these up into 3 different numbers
enum class Gamemode : int {
	UNKNOWN = -1,
	CASUAL,
	COMPETITIVE,
	WINGMAN,
	WEAPONS_EXPERT,
	GUN_GAME_ARMS_RACE,
	GUN_GAME_DEMOLITION,
	GUN_GAME_DEATHMATCH,
	TRAINING,
	CUSTOM,
	GUARDIAN,
	COOP_STRIKE,
	SKIRMISH_FLYING_SCOUTSMAN,
	SKIRMISH_ARMS_RACE,
	SKIRMISH_DEMOLITION,
	SKIRMISH_RETAKES,
	DANGER_ZONE
};

namespace Utils {
	Gamemode CalculateGamemode();
}

#endif