#include "Trigonometry.hpp"

#include <cmath>

// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1027
void Utils::AngleVectors(Vector angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	sincosf( DEG2RAD( angles[YAW] ), &sy, &cy );
	sincosf( DEG2RAD( angles[PITCH] ), &sp, &cp );
	sincosf( DEG2RAD( angles[ROLL] ), &sr, &cr );

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1*sr*sp*cy+-1*cr*-sy);
		right->y = (-1*sr*sp*sy+-1*cr*cy);
		right->z = -1*sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy+-sr*-sy);
		up->y = (cr*sp*sy+-sr*cy);
		up->z = cr*cp;
	}
}

void Utils::AngleVectors(Vector angles, Vector* forward)
{
	AngleVectors(angles, forward, nullptr, nullptr);
}

// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1108
void Utils::VectorAngles(Vector& forward, Vector& angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0) {
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	} else {
		yaw = RAD2DEG(atan2(forward[1], forward[0]));
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = RAD2DEG(atan2(-forward[2], tmp));
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

Vector Utils::CalculateView(const Vector a, const Vector b)
{
	Vector delta = b - a;
	Vector rotation {};
	Utils::VectorAngles(delta, rotation);
	return rotation;
}
