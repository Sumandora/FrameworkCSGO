#ifndef SDK_RAYTRACE_RAY
#define SDK_RAYTRACE_RAY

#include "../Math/Matrix3x4.hpp"

#include "Trace.hpp"

struct Ray {
	AlignedVector m_Start;
	AlignedVector m_Delta;
	AlignedVector m_StartOffset;
	AlignedVector m_Extents;
	Matrix3x4*	  m_pWorldAxisTransform;
	bool		  m_IsRay;
	bool		  m_IsSwept;

	Ray()
		: m_pWorldAxisTransform(nullptr) { }

	void Init(Vector& start, Vector& end) {
		m_Delta		= end - start;
		m_IsSwept	= (m_Delta.LengthSquared() != 0);
		m_Extents.x = m_Extents.y = m_Extents.z = 0;
		m_pWorldAxisTransform					= nullptr;
		m_IsRay									= true;
		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0;
		m_Start												= start;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_Delta				  = end - start;
		m_pWorldAxisTransform = nullptr;
		m_IsSwept			  = (m_Delta.LengthSquared() != 0);
		m_Extents			  = (maxs - mins) * 0.5;
		m_IsRay				  = (m_Extents.LengthSquared() < 1e-6);
		m_StartOffset		  = (maxs + mins) * 0.5;
		m_Start				  = (start + m_StartOffset) * -1.0;
	}
};

#endif
