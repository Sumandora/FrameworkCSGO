#ifndef SDK_CUSERCMD
#define SDK_CUSERCMD

#include "Vector.hpp"

struct CUserCmd
{
	virtual ~CUserCmd() {};
	int command_number;
	int tick_count;
	Vector viewangles;
	Vector aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	Vector headangles;
	Vector headoffset;
};

#endif
