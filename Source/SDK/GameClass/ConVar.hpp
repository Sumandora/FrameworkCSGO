#ifndef SDK_GAMECLASS_CONVAR
#define SDK_GAMECLASS_CONVAR

#include "Padding.hpp"
#include "VirtualMethod.hpp"

#include "../CUtl/Vector.hpp"

class ConVar;

using FnChangeCallback_t = void (*)(ConVar* var, const char* pOldValue, float flOldValue);

class ConVar {
public:
	PADDING(0x30);
	CUtlVector<FnChangeCallback_t> m_fnChangeCallbacks;

	VIRTUAL_METHOD(15, GetFloat, float, (), (this))
	VIRTUAL_METHOD(16, GetInt, int, (), (this))
	// Setting convars is discouraged

	inline bool GetBool()
	{
		return GetInt();
	}
};

#endif
