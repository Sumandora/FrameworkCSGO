#ifndef SDK_GAMECLASS_PANORAMA_UIPANEL
#define SDK_GAMECLASS_PANORAMA_UIPANEL

#include "../VirtualMethod.hpp"

class CUIPanel {
public:
	VIRTUAL_METHOD(10, GetID, const char*, (), (this))
	VIRTUAL_METHOD(26, GetParent, CUIPanel*, (), (this))
};

#endif