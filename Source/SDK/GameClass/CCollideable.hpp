#ifndef SDK_GAMECLASS_CCOLLIDEABLE
#define SDK_GAMECLASS_CCOLLIDEABLE

#include "../Math/Vector.hpp"
#include "VirtualMethod.hpp"

class CCollideable {
public:
	VIRTUAL_METHOD(1, ObbMins, Vector*, (), (this))
	VIRTUAL_METHOD(2, ObbMaxs, Vector*, (), (this))
};

#endif
