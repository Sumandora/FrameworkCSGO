#include "GameMode.hpp"

#include "../ConVarStorage.hpp"

Gamemode Utils::CalculateGamemode()
{
	const int game_type = ConVarStorage::game_type()->GetInt();
	const int game_mode = ConVarStorage::game_mode()->GetInt();
	const int sv_skirmish_id = ConVarStorage::sv_skirmish_id()->GetInt();

	// https://developer.valvesoftware.com/wiki/CS:GO_Game_Modes

	switch (game_type) {
	case 0:
		switch (game_mode) {
		case 0:
			return Gamemode::CASUAL;
		case 1:
			return Gamemode::COMPETITIVE;
		case 2:
			return Gamemode::WINGMAN;
		case 3:
			return Gamemode::WEAPONS_EXPERT;
		default:
			return Gamemode::UNKNOWN;
		}
	case 1:
		switch (game_mode) {
		case 0:
			return Gamemode::GUN_GAME_ARMS_RACE;
		case 1:
			return Gamemode::GUN_GAME_DEMOLITION;
		case 2:
			return Gamemode::GUN_GAME_DEATHMATCH;
		default:
			return Gamemode::UNKNOWN;
		}
	case 2:
		if (game_mode == 0)
			return Gamemode::TRAINING;
		else
			return Gamemode::UNKNOWN;
	case 3:
		if (game_mode == 0)
			return Gamemode::CUSTOM;
		else
			return Gamemode::UNKNOWN;
	case 4:
		if (game_mode == 0)
			return Gamemode::GUARDIAN;
		else if (game_mode == 1)
			return Gamemode::COOP_STRIKE;
		else
			return Gamemode::UNKNOWN;
	case 5:
		if (game_mode == 0) {
			switch (sv_skirmish_id) {
			case 3:
				return Gamemode::SKIRMISH_FLYING_SCOUTSMAN;
			case 10:
				return Gamemode::SKIRMISH_ARMS_RACE;
			case 11:
				return Gamemode::SKIRMISH_DEMOLITION;
			case 12:
				return Gamemode::SKIRMISH_RETAKES;
			default:
				return Gamemode::UNKNOWN;
			}
		} else
			return Gamemode::UNKNOWN;
	case 6:
		if (game_mode == 0)
			return Gamemode::DANGER_ZONE;
		else
			return Gamemode::UNKNOWN;
	default:
		return Gamemode::UNKNOWN;
	}
}