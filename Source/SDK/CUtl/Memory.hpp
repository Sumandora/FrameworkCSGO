#ifndef SDK_CUTL_MEMORY
#define SDK_CUTL_MEMORY

#include <cstdlib>

template <typename Type>
class CUtlMemory {
public:
	inline Type& operator[](int i)
	{
		return m_pMemory[i];
	}

	Type* m_pMemory = (Type*)malloc(m_nAllocationCount * sizeof(Type));
	int m_nAllocationCount = 0;
	int m_nGrowSize = 0;
};

#endif