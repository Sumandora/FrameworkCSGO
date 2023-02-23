#ifndef SDK_RAYTRACE_TRACEFILTER
#define SDK_RAYTRACE_TRACEFILTER

#include "../GameClass/CBaseEntity.hpp"

#include "TraceFilter.hpp"
#include "TraceType.hpp"

// Valve expects you to make your own implementation of this and pass it to the TraceRay function

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(/*IHandleEntity*/ void* pEntity, int contentsMask) = 0;
	virtual TraceType GetTraceType() const = 0;
};

class CTraceFilterEntity : public ITraceFilter {

	CBaseEntity* entity;

	bool ShouldHitEntity(/*IHandleEntity*/ void* pEntity, int contentsMask) {
		return pEntity != entity;
	}

	TraceType GetTraceType() const {
		return TRACE_EVERYTHING;
	}

public:
	inline CTraceFilterEntity(CBaseEntity* entity) {
		this->entity = entity;
	}
};

#endif
