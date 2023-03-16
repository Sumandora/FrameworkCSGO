#include "Raytrace.hpp"

#include "../Interfaces.hpp"

#include "../SDK/Raytrace/Definitions/MaskFlags.hpp"

Trace Utils::TraceRay(Vector start, Vector end, ITraceFilter* filter)
{
	Ray ray {};
	ray.Init(start, end);

	Trace trace {};

	Interfaces::engineTrace->TraceRay(&ray, MASK_SHOT, filter, &trace);

	return trace;
}
