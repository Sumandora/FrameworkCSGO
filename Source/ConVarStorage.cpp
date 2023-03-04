#include "ConVarStorage.hpp"

#include "Interfaces.hpp"

void ConVarStorage::FindAll() {
    weapon_recoil_scale = Interfaces::icvar->FindVar("weapon_recoil_scale");
}