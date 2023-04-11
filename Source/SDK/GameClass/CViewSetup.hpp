#ifndef SDK_CVIEWSETUP
#define SDK_CVIEWSETUP

#include "xorstr.hpp"

#include "../Math/Vector.hpp"

class CViewSetup {
	char pad[184];

public:
	float fov;
	float fovViewmodel;
	Vector origin;
	Vector angles;
	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;
	float m_flAspectRatio;
};

#endif