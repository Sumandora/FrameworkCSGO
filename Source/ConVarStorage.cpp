#include "ConVarStorage.hpp"

#include "xorstr.hpp"

#include "Interfaces.hpp"

void ConVarStorage::FindAll() {
    weapon_recoil_scale			= Interfaces::icvar->FindVar(xorstr_("weapon_recoil_scale"));
    mp_teammates_are_enemies	= Interfaces::icvar->FindVar(xorstr_("mp_teammates_are_enemies"));

    game_type		= Interfaces::icvar->FindVar(xorstr_("game_type"));
    game_mode		= Interfaces::icvar->FindVar(xorstr_("game_mode"));
    sv_skirmish_id	= Interfaces::icvar->FindVar(xorstr_("sv_skirmish_id"));
}