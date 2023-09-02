#ifndef UTILS_TRIGONOMETRY
#define UTILS_TRIGONOMETRY

#include <cmath>

#include "../SDK/Math/Vector.hpp"

#define DEG2RAD(deg) ((deg)*M_PI / 180.0)
#define RAD2DEG(rad) ((rad)*180.0 / M_PI)

enum {
	PITCH = 0,
	YAW,
	ROLL
};

namespace Utils {
	void angleVectors(const Vector& angles, Vector* forward);
	void angleVectors(Vector angles, Vector* forward, Vector* right, Vector* up);
	void vectorAngles(const Vector& forward, Vector& angles);
	Vector calculateView(const Vector& a, const Vector& b);
}

#endif