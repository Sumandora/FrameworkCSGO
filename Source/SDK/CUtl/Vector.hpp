#ifndef SDK_CUTL_VECTOR
#define SDK_CUTL_VECTOR

#include "Memory.hpp"

template <typename Type>
class CUtlVector {
public:
	inline Type& operator[](int i)
	{
		return m_Memory[i];
	}

	inline int Count() const
	{
		return m_Size;
	}

	CUtlMemory<Type> m_Memory;
	int m_Size = 0;
	Type* m_pElements;
};

#endif