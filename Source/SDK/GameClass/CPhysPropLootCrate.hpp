#ifndef SDK_GAMECLASS_CPHYSPROPLOOTCRATE
#define SDK_GAMECLASS_CPHYSPROPLOOTCRATE

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CPhysPropLootCrate : public CBaseEntity {
public:
	NETVAR_FUNCTION(CBaseHandle, Health, ClientClassID::CPhysPropLootCrate, xorstr_("DT_PhysPropLootCrate"), xorstr_("m_iHealth"))
	NETVAR_FUNCTION(CBaseHandle, MaxHealth, ClientClassID::CPhysPropLootCrate, xorstr_("DT_PhysPropLootCrate"), xorstr_("m_iMaxHealth"))
};

#endif
