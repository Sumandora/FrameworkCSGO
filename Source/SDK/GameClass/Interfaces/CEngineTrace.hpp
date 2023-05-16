#ifndef SDK_GAMECLASS_INTERFACES_CENGINETRACE
#define SDK_GAMECLASS_INTERFACES_CENGINETRACE

#include "SDK/GameClass/VirtualMethod.hpp"

#include "SDK/Raytrace/Ray.hpp"
#include "SDK/Raytrace/TraceFilter.hpp"

class CEngineTrace {
public:
	VIRTUAL_METHOD(5, TraceRay, void, (Ray & ray, unsigned int fMask, ITraceFilter* pTraceFilter, Trace* pTrace), (this, ray, fMask, pTraceFilter, pTrace))
};

#endif
