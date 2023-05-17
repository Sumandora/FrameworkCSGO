#ifndef SDK_GAMECLASS_INTERFACES_ICVAR
#define SDK_GAMECLASS_INTERFACES_ICVAR

#include "../ConVar.hpp"

class ICvar {
public:
	VIRTUAL_METHOD(15, FindVar, ConVar*, (const char* name), (this, name))
};

#endif
