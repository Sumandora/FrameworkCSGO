#ifndef UTILS_TRIGONOMETRY
#define UTILS_TRIGONOMETRY

#include <cmath>

#include "../SDK/Math/Vector.hpp"

#define DEG2RAD(deg) ((deg)*M_PI / 180.0)
#define RAD2DEG(rad) ((rad)*180.0 / M_PI)

namespace Utils {
	void AngleVectors(Vector angles, Vector* forward);
	void VectorAngles(Vector& forward, Vector& angles);
	Vector CalculateView(Vector a, Vector b);
}

#endif