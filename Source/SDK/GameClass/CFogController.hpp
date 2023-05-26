#ifndef SDK_GAMECLASS_CFOGCONTROLLER
#define SDK_GAMECLASS_CFOGCONTROLLER

#include "xorstr.hpp"

#include "../../Netvars.hpp"
#include "VirtualMethod.hpp"

#include "CBaseEntity.hpp"

class CFogController : public CBaseEntity {
public:
    NETVAR_FUNCTION(bool, Enable, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.enable"))
    NETVAR_FUNCTION(bool, Blend, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.blend"))
    NETVAR_FUNCTION(Vector, DirPrimary, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.dirPrimary"))
    NETVAR_FUNCTION(std::uint32_t, ColorPrimary, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.colorPrimary"))
    NETVAR_FUNCTION(std::uint32_t, ColorSecondary, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.colorSecondary"))
    NETVAR_FUNCTION(float, Start, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.start"))
    NETVAR_FUNCTION(float, End, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.end"))
    NETVAR_FUNCTION(float, FarZ, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.farz"))
    NETVAR_FUNCTION(float, MaxDensity, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.maxdensity"))
    NETVAR_FUNCTION(float, Duration, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.duration"))
    NETVAR_FUNCTION(float, HDRColorScale, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.HDRColorScale"))
    NETVAR_FUNCTION(float, ZoomFogScale, ClientClassID::CFogController, xorstr_("DT_FogController"), xorstr_("m_fog.ZoomFogScale"))
};

#endif
