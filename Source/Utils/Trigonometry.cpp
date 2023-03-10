#include "Trigonometry.hpp"

#include <cmath>

// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1009
void Utils::AngleVectors(Vector angles, Vector* forward)
{
	float pitch = DEG2RAD(angles.x);
	float yaw = DEG2RAD(angles.y);

	forward->x = cos(pitch) * cos(yaw);
	forward->y = cos(pitch) * sin(yaw);
	forward->z = -sin(pitch);
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
	Vector rotation = {};
	Utils::VectorAngles(delta, rotation);
	return rotation;
}
