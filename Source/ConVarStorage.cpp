#include "ConVarStorage.hpp"

#include "Interfaces.hpp"

#define INIT_LAZY_CONVAR(name)                                   \
	ConVar* ConVarStorage::name()                                \
	{                                                            \
		static ConVar* convar = nullptr;                         \
		if (!convar) {                                           \
			convar = Interfaces::icvar->FindVar(xorstr_(#name)); \
		}                                                        \
		return convar;                                           \
	}

// Combat
INIT_LAZY_CONVAR(mp_teammates_are_enemies)

// Gamemode
INIT_LAZY_CONVAR(game_mode)

// Pitch
INIT_LAZY_CONVAR(cl_pitchup)
INIT_LAZY_CONVAR(cl_pitchdown)

// Lag Compensation
INIT_LAZY_CONVAR(cl_lagcompensation)
// Unlag
INIT_LAZY_CONVAR(sv_unlag)
INIT_LAZY_CONVAR(sv_maxunlag)
// Lerping
INIT_LAZY_CONVAR(cl_updaterate)
INIT_LAZY_CONVAR(sv_minupdaterate)
INIT_LAZY_CONVAR(sv_maxupdaterate)
INIT_LAZY_CONVAR(cl_interpolate)
INIT_LAZY_CONVAR(cl_interp_ratio)
INIT_LAZY_CONVAR(cl_interp)
INIT_LAZY_CONVAR(sv_client_min_interp_ratio)
INIT_LAZY_CONVAR(sv_client_max_interp_ratio)

// Movement
INIT_LAZY_CONVAR(sv_maxspeed)
INIT_LAZY_CONVAR(sv_airaccelerate)
INIT_LAZY_CONVAR(sv_air_max_wishspeed)

// Mouse
INIT_LAZY_CONVAR(m_yaw)
INIT_LAZY_CONVAR(m_pitch)