#ifndef SDK_MODEL
#define SDK_MODEL

#include "ModType.hpp"
#include "../Math/Vector.hpp"

struct Model {
	void* fnHandle;
	char szPathName[260];

	int nLoadFlags;
	int nServerCount;

	ModType modType;
	int flags;

	Vector mins, maxs;
};

#endif