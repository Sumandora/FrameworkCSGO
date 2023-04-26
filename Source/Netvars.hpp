#ifndef NETVARS
#define NETVARS

#include <map>
#include <vector>

#include "SDK/ClientClassIDs.hpp"

#include "SDK/Netvars/ClientClass.hpp"
#include "SDK/Netvars/RecvTable.hpp"

#define NETVAR_FUNCTION(returnType, functionName, clientClass, table, name) \
	returnType* functionName()                                              \
	{                                                                       \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)  \
			+ Netvars::GetOffset(clientClass, table, name)->m_Offset);      \
	}
#define COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2) \
	returnType* functionName()                                                                                       \
	{                                                                                                                \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)                                           \
			+ Netvars::GetOffset(clientClass1, table1, name1)->m_Offset                                              \
			+ Netvars::GetOffset(clientClass2, table2, name2)->m_Offset);                                            \
	}
#define TRIPLE_COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2, clientClass3, table3, name3) \
	returnType* functionName()                                                                                                                           \
	{                                                                                                                                                    \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)                                                                               \
			+ Netvars::GetOffset(clientClass1, table1, name1)->m_Offset                                                                                  \
			+ Netvars::GetOffset(clientClass2, table2, name2)->m_Offset                                                                                  \
			+ Netvars::GetOffset(clientClass3, table3, name3)->m_Offset);                                                                                \
	}

namespace Netvars {
	inline std::map<ClientClass*, std::map<RecvTable*, std::vector<RecvProp*>>> netvars{};

	void DumpNetvars();
	RecvProp* GetOffset(ClientClassID clientClass, const char* table, const char* name);
}

#endif
