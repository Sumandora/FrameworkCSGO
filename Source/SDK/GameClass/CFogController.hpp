#ifndef SDK_GAMECLASS_CFOGCONTROLLER
#define SDK_GAMECLASS_CFOGCONTROLLER

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CFogController : public CBaseEntity {
public:
	NETVAR_FUNCTION(bool, Enable, ClientClassID::CFogController, "DT_FogController", "m_fog.enable")
	NETVAR_FUNCTION(bool, Blend, ClientClassID::CFogController, "DT_FogController", "m_fog.blend")
	NETVAR_FUNCTION(Vector, DirPrimary, ClientClassID::CFogController, "DT_FogController", "m_fog.dirPrimary")
	NETVAR_FUNCTION(std::uint32_t, ColorPrimary, ClientClassID::CFogController, "DT_FogController", "m_fog.colorPrimary")
	NETVAR_FUNCTION(std::uint32_t, ColorSecondary, ClientClassID::CFogController, "DT_FogController", "m_fog.colorSecondary")
	NETVAR_FUNCTION(float, Start, ClientClassID::CFogController, "DT_FogController", "m_fog.start")
	NETVAR_FUNCTION(float, End, ClientClassID::CFogController, "DT_FogController", "m_fog.end")
	NETVAR_FUNCTION(float, FarZ, ClientClassID::CFogController, "DT_FogController", "m_fog.farz")
	NETVAR_FUNCTION(float, MaxDensity, ClientClassID::CFogController, "DT_FogController", "m_fog.maxdensity")
	NETVAR_FUNCTION(float, Duration, ClientClassID::CFogController, "DT_FogController", "m_fog.duration")
	NETVAR_FUNCTION(float, HDRColorScale, ClientClassID::CFogController, "DT_FogController", "m_fog.HDRColorScale")
	NETVAR_FUNCTION(float, ZoomFogScale, ClientClassID::CFogController, "DT_FogController", "m_fog.ZoomFogScale")
};

#endif
