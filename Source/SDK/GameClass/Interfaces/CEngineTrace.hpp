#ifndef SDK_CENGINETRACE
#define SDK_CENGINETRACE

#include "../VirtualMethod.hpp"

#include "../../Raytrace/Ray.hpp"
#include "../../Raytrace/TraceFilter.hpp"

class CEngineTrace {
public:
	VIRTUAL_METHOD(5, TraceRay, void, (Ray* ray, unsigned int fMask, ITraceFilter* pTraceFilter, Trace* pTrace), (this, ray, fMask, pTraceFilter, pTrace))
};

#endif
