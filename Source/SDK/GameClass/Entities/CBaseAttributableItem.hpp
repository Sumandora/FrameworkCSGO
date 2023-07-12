#ifndef SDK_GAMECLASS_CBASEATTRIBUTABLEITEM
#define SDK_GAMECLASS_CBASEATTRIBUTABLEITEM

#include "../../../Netvars.hpp"
#include "../../Weapons/WeaponIDs.hpp"

#include "CBaseEntity.hpp"

class CBaseAttributableItem : public CBaseEntity {
public:
	TRIPLE_COMBINED_NETVAR_FUNCTION(WeaponID, WeaponDefinitionIndex,
		ClientClassID::CBaseAttributableItem, "DT_BaseAttributableItem", "m_AttributeManager",
		ClientClassID::CBaseAttributableItem, "DT_AttributeContainer", "m_Item",
		ClientClassID::CBaseAttributableItem, "DT_ScriptCreatedItem", "m_iItemDefinitionIndex")
};

#endif
