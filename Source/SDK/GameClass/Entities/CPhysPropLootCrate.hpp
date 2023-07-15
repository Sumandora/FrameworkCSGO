#ifndef SDK_GAMECLASS_ENTITIES_CPHYSPROPLOOTCRATE
#define SDK_GAMECLASS_ENTITIES_CPHYSPROPLOOTCRATE

#include "../../../Netvars.hpp"
#include "../VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CPhysPropLootCrate : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, Health, ClientClassID::CPhysPropLootCrate, "DT_PhysPropLootCrate", "m_iHealth")
	NETVAR_FUNCTION(int, MaxHealth, ClientClassID::CPhysPropLootCrate, "DT_PhysPropLootCrate", "m_iMaxHealth")
};

#endif
