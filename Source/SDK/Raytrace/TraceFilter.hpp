#ifndef SDK_RAYTRACE_TRACEFILTER
#define SDK_RAYTRACE_TRACEFILTER

#include "../GameClass/CBaseEntity.hpp"

#include "TraceFilter.hpp"
#include "TraceType.hpp"

// Valve expects you to make your own implementation of this and pass it to the TraceRay function

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	[[nodiscard]] virtual TraceType GetTraceType() const = 0;
};

class CTraceFilterNoEntities : public ITraceFilter {
	bool ShouldHitEntity(void* pEntity, int contentsMask) override
	{
		return false; // This is not being invoked.
	}

	[[nodiscard]] TraceType GetTraceType() const override
	{
		return TRACE_WORLD_ONLY;
	}
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
