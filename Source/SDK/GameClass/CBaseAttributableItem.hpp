#ifndef SDK_CBASEATTRIBUTABLEITEM
#define SDK_CBASEATTRIBUTABLEITEM

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "../WeaponIDs.hpp"

#include "CBaseEntity.hpp"

class CBaseAttributableItem : public CBaseEntity {
public:
	TRIPLE_COMBINED_NETVAR_FUNCTION(WeaponID, WeaponDefinitionIndex,
		ClientClassID::CBaseAttributableItem, xorstr_("DT_BaseAttributableItem"), xorstr_("m_AttributeManager"),
		ClientClassID::CBaseAttributableItem, xorstr_("DT_AttributeContainer"), xorstr_("m_Item"),
		ClientClassID::CBaseAttributableItem, xorstr_("DT_ScriptCreatedItem"), xorstr_("m_iItemDefinitionIndex"))
};

#endif
