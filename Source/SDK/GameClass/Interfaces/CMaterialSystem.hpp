#ifndef SDK_GAMECLASS_INTERFACES_CMATERIALSYSTEM
#define SDK_GAMECLASS_INTERFACES_CMATERIALSYSTEM

#include "SDK/GameClass/VirtualMethod.hpp"

#include "SDK/GameClass/IMaterial.hpp"

class CMaterialSystem {
public:
	VIRTUAL_METHOD(84, FindMaterial, IMaterial*, (const char *materialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = nullptr), (this, materialName, pTextureGroupName, complain, pComplainPrefix))
};

#endif
