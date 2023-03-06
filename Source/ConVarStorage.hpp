#ifndef CONVARSTORAGE
#define CONVARSTORAGE

#include "SDK/GameClass/ConVar.hpp"

namespace ConVarStorage {
    inline ConVar* weapon_recoil_scale;
    inline ConVar* mp_teammates_are_enemies;

    inline ConVar* game_type;
    inline ConVar* game_mode;
    inline ConVar* sv_skirmish_id;
    
    void FindAll();
}

#endif