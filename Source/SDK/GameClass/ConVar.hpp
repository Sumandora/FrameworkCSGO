#ifndef SDK_GAMECLASS_CONVAR
#define SDK_GAMECLASS_CONVAR

#include "VirtualMethod.hpp"

class ConVar {
public:
	VIRTUAL_METHOD(15, GetFloat, float, (), (this))
	VIRTUAL_METHOD(16, GetInt, int, (), (this))
	// Setting convars is discouraged

	inline bool GetBool()
	{
		return GetInt();
	}
};

#endif
