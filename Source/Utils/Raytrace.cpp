#include "Raytrace.hpp"

#include "../Interfaces.hpp"

#include "../SDK/Math/Vector.hpp"

#include "../SDK/GameClass/Interfaces/CEngineTrace.hpp"

#include "../SDK/Raytrace/Definitions/MaskFlags.hpp"
#include "../SDK/Raytrace/Ray.hpp"

class ITraceFilter;

Trace Utils::TraceRay(Vector start, Vector end, ITraceFilter* filter) {
	Ray ray;
	ray.Init(start, end);

	Trace trace;
	Interfaces::engineTrace->TraceRay(&ray, MASK_SHOT, filter, &trace);

	return trace;
}
