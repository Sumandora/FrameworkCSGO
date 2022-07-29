#ifndef SDK_ICLIENTRENDERABLE
#define SDK_ICLIENTRENDERABLE

#include "VirtualMethod.hpp"
#include "../Matrix3x4.hpp"

struct IClientRenderable {
public:
	VIRTUAL_METHOD(13, SetupBones, bool, (matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0), (this + sizeof(uintptr_t), pBoneMatrix, nMaxBones, nBoneMask, flCurTime))
};

#endif
