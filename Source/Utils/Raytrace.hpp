#ifndef UTILS_RAYTRACE
#define UTILS_RAYTRACE

#include "../SDK/Math/Vector.hpp"

#include "../SDK/Raytrace/Ray.hpp"
#include "../SDK/Raytrace/TraceFilter.hpp"
#include "../SDK/Raytrace/Trace.hpp"

class ITraceFilter;

namespace Utils {
	Trace TraceRay(Vector start, Vector end, ITraceFilter* filter);
}

#endif
