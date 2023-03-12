#ifndef NETVARS
#define NETVARS

#include "SDK/Netvars/RecvProp.hpp"

#define NETVAR_FUNCTION(returnType, functionName, table, name)             \
	returnType* functionName()                                             \
	{                                                                      \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this) \
			+ Netvars::GetOffset(table, name)->m_Offset);                  \
	}
#define COMBINED_NETVAR_FUNCTION(returnType, functionName, table1, name1, table2, name2) \
	returnType* functionName()                                                           \
	{                                                                                    \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)               \
			+ Netvars::GetOffset(table1, name1)->m_Offset                                \
			+ Netvars::GetOffset(table2, name2)->m_Offset);                              \
	}
#define TRIPLE_COMBINED_NETVAR_FUNCTION(returnType, functionName, table1, name1, table2, name2, table3, name3) \
	returnType* functionName()                                                                                 \
	{                                                                                                          \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)                                     \
			+ Netvars::GetOffset(table1, name1)->m_Offset                                                      \
			+ Netvars::GetOffset(table2, name2)->m_Offset                                                      \
			+ Netvars::GetOffset(table3, name3)->m_Offset);                                                    \
	}

namespace Netvars {
	void DumpNetvars();
	RecvProp* GetOffset(const char* table, const char* name);
	void SetupGUI();
}

#endif
