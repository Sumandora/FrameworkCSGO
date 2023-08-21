#ifndef UTILS_PROJECTION
#define UTILS_PROJECTION

#include "../SDK/Math/Vector.hpp"
#include "imgui.h"

namespace Utils {
	bool project(const Vector& world, ImVec2& screen);
}

#endif
