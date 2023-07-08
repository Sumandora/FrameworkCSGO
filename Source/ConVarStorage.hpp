#ifndef CONVARSTORAGE
#define CONVARSTORAGE

#include "SDK/GameClass/ConVar.hpp"

#define LAZY_CONVAR(name)        \
	inline ConVar* name()        \
	{                            \
		return GetConVar(#name); \
	}

namespace ConVarStorage {

	ConVar* GetConVar(const char* name);

	// Combat
	LAZY_CONVAR(mp_teammates_are_enemies)

	// Gamemode
	LAZY_CONVAR(game_type)

	// Pitch
	LAZY_CONVAR(cl_pitchup)
	LAZY_CONVAR(cl_pitchdown)

	// Lag Compensation
	LAZY_CONVAR(cl_lagcompensation)
	// Unlag
	LAZY_CONVAR(sv_unlag)
	LAZY_CONVAR(sv_maxunlag)
	// Lerping
	LAZY_CONVAR(cl_updaterate)
	LAZY_CONVAR(sv_minupdaterate)
	LAZY_CONVAR(sv_maxupdaterate)
	LAZY_CONVAR(cl_interpolate)
	LAZY_CONVAR(cl_interp_ratio)
	LAZY_CONVAR(cl_interp)
	LAZY_CONVAR(sv_client_min_interp_ratio)
	LAZY_CONVAR(sv_client_max_interp_ratio)

	// Movement
	LAZY_CONVAR(sv_maxspeed)
	LAZY_CONVAR(sv_airaccelerate)
	LAZY_CONVAR(sv_air_max_wishspeed)
	LAZY_CONVAR(cl_forwardspeed)
	LAZY_CONVAR(cl_sidespeed)

	// Mouse
	LAZY_CONVAR(m_yaw)
	LAZY_CONVAR(m_pitch)
}

#endif