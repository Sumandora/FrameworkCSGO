#include "Trigonometry.hpp"

#include <cmath>

// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1027
void Utils::angleVectors(Vector angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	sincosf(DEG2RAD(angles[YAW]), &sy, &cy);
	sincosf(DEG2RAD(angles[PITCH]), &sp, &cp);
	sincosf(DEG2RAD(angles[ROLL]), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

void Utils::angleVectors(const Vector& angles, Vector* forward)
{
	angleVectors(angles, forward, nullptr, nullptr);
}

// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/mathlib/mathlib_base.cpp#L1108
void Utils::vectorAngles(const Vector& forward, Vector& angles)
{
	float tmp, yaw, pitch;

	if (forward.x == 0 && forward.y == 0) {
		yaw = 0;
		if (forward.z > 0)
			pitch = -90;
		else
			pitch = 90;
	} else {
		yaw = RAD2DEG(std::atan2(forward.y, forward.x));
		tmp = std::sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = RAD2DEG(std::atan2(-forward.z, tmp));
	}

	angles[PITCH] = pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

Vector Utils::calculateView(const Vector& a, const Vector& b)
{
	Vector delta = b - a;
	Vector rotation{};
	Utils::vectorAngles(delta, rotation);
	return rotation;
}
