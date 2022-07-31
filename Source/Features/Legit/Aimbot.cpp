#include "Aimbot.hpp"

#include "../../Interfaces.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#define DEG2RAD(deg) (deg * M_PI / 180.0)
#define RAD2DEG(rad) (rad * 180.0 / M_PI)

// Thanks 2 Mathlib (https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1108)
void VectorAngles( const Vector& forward, Vector &angles )
{
	float	tmp, yaw, pitch;
	
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

Vector CalculateView(Vector a, Vector b) {
	Vector delta = b - a;
	Vector rotation = {};
	VectorAngles(delta, rotation);
	return rotation;
}

void NormalizeAngles(Vector& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

void Features::Legit::Aimbot::PollEvent(SDL_Event* event) {
	if(event->type != SDL_MOUSEMOTION)
		return;
	
	if(!Interfaces::engine->IsInGame())
		return;
	
	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	C_BasePlayer* localPlayer = reinterpret_cast<C_BasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));

	if(!localPlayer)
		return;

	TeamID localTeam = *localPlayer->Team();
	if(
		localTeam == TeamID::TEAM_UNASSIGNED ||
		localTeam == TeamID::TEAM_SPECTATOR
	) return;

	Vector playerEye = localPlayer->GetEyePosition();
	
	Vector viewAngles;
	Interfaces::engine->GetViewAngles(&viewAngles);
	
	C_BasePlayer* target = nullptr;
	float bestDistance;
	Vector bestRotation;

	
	// The first object is always the WorldObj
	for(int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		C_BasePlayer* player = reinterpret_cast<C_BasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if(!player ||
			player == localPlayer ||
			player->GetDormant() ||
			*player->LifeState() != LIFE_ALIVE ||
			*player->GunGameImmunity() ||
			*player->Team() == localTeam ||
			*player->SpottedByMask() & (1 << localPlayerIndex)
		) continue;

		Vector rotation = CalculateView(playerEye, player->GetBonePosition(8)) - viewAngles;
		NormalizeAngles(rotation);
		float delta = rotation.Length();
		if(!target || bestDistance > delta) {
			target = player;
			bestDistance = delta;
			bestRotation = rotation;
		}
	}

	if(!target)
		return;

	if(bestRotation.Length() > /*FOV*/ 10.0F)
		return;

	bestRotation /= 100.0f; // Approximation, this has to be changed into a rotation -> mouse delta calculation

	int beforeX = event->motion.xrel;
	int beforeY = event->motion.yrel;
	
	event->motion.xrel -= bestRotation.y;
	event->motion.yrel += bestRotation.x;

	printf(xorstr_("%d %d -> %d %d\n"), beforeX, beforeY, event->motion.xrel, event->motion.yrel);
}
