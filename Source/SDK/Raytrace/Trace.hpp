#ifndef SDK_RAYTRACE_TRACE
#define SDK_RAYTRACE_TRACE

#include "../GameClass/CBaseEntity.hpp"

#include "Plane.hpp"
#include "Surface.hpp"

struct Trace {
	// CBaseTrace
	Vector			startpos;
	Vector			endpos;
	Plane			plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	// CGameTrace
	float			fractionleftsolid;
	Surface			surface;

	int				hitgroup;

	short			physicsbone;
	unsigned short	worldSurfaceIndex;

	CBaseEntity*	m_pEnt;

	int				hitbox;
};

#endif
