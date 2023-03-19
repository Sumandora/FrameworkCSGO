#ifndef CONVARSTORAGE
#define CONVARSTORAGE

#include "SDK/GameClass/ConVar.hpp"

namespace ConVarStorage {
	// Combat
	inline ConVar* weapon_recoil_scale;
	inline ConVar* mp_teammates_are_enemies;

	// Gamemode
	inline ConVar* game_type;
	inline ConVar* game_mode;
	inline ConVar* sv_skirmish_id;

	// Pitch
	inline ConVar* cl_pitchup;
	inline ConVar* cl_pitchdown;

	// Lag Compensation
	inline ConVar* cl_lagcompensation;
	// Unlag
	inline ConVar* sv_unlag;
	inline ConVar* sv_maxunlag;
	// Lerping
	inline ConVar* cl_updaterate;
	inline ConVar* sv_minupdaterate;
	inline ConVar* sv_maxupdaterate;
	inline ConVar* cl_interpolate;
	inline ConVar* cl_interp_ratio;
	inline ConVar* cl_interp;
	inline ConVar* sv_client_min_interp_ratio;
	inline ConVar* sv_client_max_interp_ratio;

	// Movement
	inline ConVar* sv_maxspeed;
	inline ConVar* sv_airaccelerate;
	inline ConVar* sv_air_max_wishspeed;

	void FindAll();
}

#endif