#ifndef SDK_GAMECLASS_IMATERIAL
#define SDK_GAMECLASS_IMATERIAL

#include "CBaseEntity.hpp"
#include "VirtualMethod.hpp"

class IMaterial {
public:
	VIRTUAL_METHOD(0, GetName, const char*, (), (this))
	VIRTUAL_METHOD(27, AlphaModulate, void, (float alpha), (this, alpha))
	VIRTUAL_METHOD(28, ColorModulate, void, (float r, float g, float b), (this, r, g, b))
};

#endif