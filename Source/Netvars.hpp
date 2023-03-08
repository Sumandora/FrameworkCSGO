#ifndef NETVARS
#define NETVARS

#define NETVAR_FUNCTION(returnType, functionName, table, name)				\
	returnType* functionName() {											\
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)	\
		+ Netvars::GetOffset(table, name));									\
	}
#define COMBINED_NETVAR_FUNCTION(returnType, functionName, table1, name1, table2, name2)	\
	returnType* functionName() {															\
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)					\
		+ Netvars::GetOffset(table1, name1)													\
		+ Netvars::GetOffset(table2, name2));												\
	}
#define TRIPLE_COMBINED_NETVAR_FUNCTION(returnType, functionName, table1, name1, table2, name2, table3, name3)	\
	returnType* functionName() {												\
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)		\
			+ Netvars::GetOffset(table1, name1)									\
			+ Netvars::GetOffset(table2, name2)									\
			+ Netvars::GetOffset(table3, name3));								\
	}

namespace Netvars {
	void DumpNetvars();
	int	 GetOffset(const char* table, const char* name);
	void SetupGUI();
}

#endif
