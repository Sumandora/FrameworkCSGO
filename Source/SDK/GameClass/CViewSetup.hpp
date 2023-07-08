#ifndef SDK_GAMECLASS_CVIEWSETUP
#define SDK_GAMECLASS_CVIEWSETUP

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