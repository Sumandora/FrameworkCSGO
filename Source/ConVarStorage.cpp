#include "ConVarStorage.hpp"

#include "Interfaces.hpp"

void ConVarStorage::FindAll() {
    weapon_recoil_scale			= Interfaces::icvar->FindVar("weapon_recoil_scale");
    mp_teammates_are_enemies	= Interfaces::icvar->FindVar("mp_teammates_are_enemies");

    game_type		= Interfaces::icvar->FindVar("game_type");
    game_mode		= Interfaces::icvar->FindVar("game_mode");
    sv_skirmish_id	= Interfaces::icvar->FindVar("sv_skirmish_id");
}