#ifndef SDK_RAYTRACE_TRACEFILTER
#define SDK_RAYTRACE_TRACEFILTER

#include "../GameClass/Entities/CBaseEntity.hpp"

#include "TraceType.hpp"

// Valve expects you to make your own implementation of this and pass it to the TraceRay function

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	[[nodiscard]] virtual TraceType GetTraceType() const = 0;
};

class CTraceFilterEntity : public ITraceFilter {

	CBaseEntity* entity;

	bool ShouldHitEntity(void* pEntity, int contentsMask) override
	{
		return pEntity != entity;
	}

	[[nodiscard]] TraceType GetTraceType() const override
	{
		return TRACE_EVERYTHING;
	}

public:
	inline explicit CTraceFilterEntity(CBaseEntity* entity)
	{
		this->entity = entity;
	}
};

#endif
