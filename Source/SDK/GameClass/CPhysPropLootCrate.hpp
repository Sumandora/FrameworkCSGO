#ifndef SDK_GAMECLASS_CPHYSPROPLOOTCRATE
#define SDK_GAMECLASS_CPHYSPROPLOOTCRATE

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CPhysPropLootCrate : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, Health, ClientClassID::CPhysPropLootCrate, "DT_PhysPropLootCrate", "m_iHealth")
	NETVAR_FUNCTION(CBaseHandle, MaxHealth, ClientClassID::CPhysPropLootCrate, "DT_PhysPropLootCrate", "m_iMaxHealth")
};

#endif
