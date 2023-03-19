#include "ConVarStorage.hpp"

#include "xorstr.hpp"

#include "Interfaces.hpp"

void ConVarStorage::FindAll()
{
	weapon_recoil_scale = Interfaces::icvar->FindVar(xorstr_("weapon_recoil_scale"));
	mp_teammates_are_enemies = Interfaces::icvar->FindVar(xorstr_("mp_teammates_are_enemies"));

	game_type = Interfaces::icvar->FindVar(xorstr_("game_type"));
	game_mode = Interfaces::icvar->FindVar(xorstr_("game_mode"));
	sv_skirmish_id = Interfaces::icvar->FindVar(xorstr_("sv_skirmish_id"));

	cl_pitchup = Interfaces::icvar->FindVar(xorstr_("cl_pitchup"));
	cl_pitchdown = Interfaces::icvar->FindVar(xorstr_("cl_pitchdown"));

	cl_lagcompensation = Interfaces::icvar->FindVar(xorstr_("cl_lagcompensation"));

	sv_unlag = Interfaces::icvar->FindVar(xorstr_("sv_unlag"));
	sv_maxunlag = Interfaces::icvar->FindVar(xorstr_("sv_maxunlag"));

	cl_updaterate = Interfaces::icvar->FindVar(xorstr_("cl_updaterate"));
	sv_minupdaterate = Interfaces::icvar->FindVar(xorstr_("sv_minupdaterate"));
	sv_maxupdaterate = Interfaces::icvar->FindVar(xorstr_("sv_maxupdaterate"));
	cl_interpolate = Interfaces::icvar->FindVar(xorstr_("cl_interpolate"));
	cl_interp_ratio = Interfaces::icvar->FindVar(xorstr_("cl_interp_ratio"));
	cl_interp = Interfaces::icvar->FindVar(xorstr_("cl_interp"));
	sv_client_min_interp_ratio = Interfaces::icvar->FindVar(xorstr_("sv_client_min_interp_ratio"));
	sv_client_max_interp_ratio = Interfaces::icvar->FindVar(xorstr_("sv_client_max_interp_ratio"));

	sv_maxspeed = Interfaces::icvar->FindVar(xorstr_("sv_maxspeed"));
	sv_airaccelerate = Interfaces::icvar->FindVar(xorstr_("sv_airaccelerate"));
	sv_air_max_wishspeed = Interfaces::icvar->FindVar(xorstr_("sv_air_max_wishspeed"));
}