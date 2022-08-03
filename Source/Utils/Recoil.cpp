#include "Recoil.hpp"

#include "../Interfaces.hpp"
#include "../Memory.hpp"

Vector Utils::CalcRecoilKickBack(C_BasePlayer* player) {
	static Vector prevAimPunch = {};
	
	if(*player->ShotsFired() <= 1) {
		prevAimPunch = {};
		return {};
	}

	Vector recoil = *player->AimPunchAngle() - prevAimPunch;
	prevAimPunch = *player->AimPunchAngle();
	return recoil;
}
