#ifndef SDK_CBASEATTRIBUTABLEITEM
#define SDK_CBASEATTRIBUTABLEITEM

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "../WeaponIDs.hpp"

#include "CBaseEntity.hpp"

class CBaseAttributableItem {
public:
	NETVAR_FUNCTION(WeaponID, WeaponDefinitionIndex, xorstr_("DT_ScriptCreatedItem"), xorstr_("m_iItemDefinitionIndex"))
};

#endif
