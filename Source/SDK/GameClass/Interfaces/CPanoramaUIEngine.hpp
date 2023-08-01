#ifndef SDK_GAMECLASS_INTERFACES_CPANORAMAUIENGINE
#define SDK_GAMECLASS_INTERFACES_CPANORAMAUIENGINE

#include "../Panorama/CUIEngine.hpp"

class CPanoramaUIEngine {
public:
	PADDING(8);
	CUIEngine* m_pUIEngine;

	VIRTUAL_METHOD(11, AccessUIEngine, CUIEngine*, (), (this))
};

#endif