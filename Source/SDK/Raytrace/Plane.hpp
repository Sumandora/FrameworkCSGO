#ifndef SDK_RAYTRACE_PLANE
#define SDK_RAYTRACE_PLANE

#include "../Math/Vector.hpp"

struct Plane {
	Vector normal;
	float dist;
	unsigned char type;
	unsigned char signbits;

	// This really is just a padding, also in Source Engine
	unsigned char pad[2];
};

#endif
