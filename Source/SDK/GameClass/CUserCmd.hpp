#ifndef SDK_CUSERCMD
#define SDK_CUSERCMD

#include "../Math/Vector.hpp"

class CUserCmd {
	virtual ~CUserCmd() = default;;
public:
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
	short mousedx; // TODO Fix these
	short mousedy;
	bool hasbeenpredicted;
	Vector viewangles_copy;
	int buttons_copy;
};

#endif
