#ifndef SDK_GAMECLASS_CPLANTEDC4
#define SDK_GAMECLASS_CPLANTEDC4

#include "../../../Netvars.hpp"
#include "../VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CPlantedC4 : public CBaseEntity {
public:
	NETVAR_FUNCTION(int, BombSite, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_nBombSite")

	NETVAR_FUNCTION(bool, BombTicking, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_bBombTicking")
	NETVAR_FUNCTION(float, BombTime, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_flC4Blow")
	NETVAR_FUNCTION(float, TimerLength, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_flTimerLength")

	NETVAR_FUNCTION(CBaseHandle, Defuser, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_hBombDefuser")
	NETVAR_FUNCTION(bool, Defused, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_bBombDefused")
	NETVAR_FUNCTION(float, DefuseCountDown, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_flDefuseCountDown")
	NETVAR_FUNCTION(float, DefuseLength, ClientClassID::CPlantedC4, "DT_PlantedC4", "m_flDefuseLength")
};

#endif
