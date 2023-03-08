#ifndef SDK_CCPLANTEDC4
#define SDK_CCPLANTEDC4

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CPlantedC4 : public CBaseEntity {
public:
	NETVAR_FUNCTION(float, BombTime, xorstr_("DT_PlantedC4"), xorstr_("m_flC4Blow"))
	//TODO
};

#endif
