#ifndef SDK_CUTL_MEMORY
#define SDK_CUTL_MEMORY

template <typename Type>
class CUtlMemory {
public:
	inline Type& operator[](int i)
	{
		return m_pMemory[i];
	}

	Type* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

#endif