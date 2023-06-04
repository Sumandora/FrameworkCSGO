#ifndef NETVARS
#define NETVARS

#include <map>
#include <string>
#include <vector>

#include "SDK/ClientClassIDs.hpp"

#include "SDK/Netvars/ClientClass.hpp"
#include "SDK/Netvars/RecvTable.hpp"

#define NETVAR_FUNCTION(returnType, functionName, clientClass, table, name) \
	inline returnType* functionName()                                       \
	{                                                                       \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)  \
			+ Netvars::GetOffset(clientClass, table, name));                \
	}
#define COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2) \
	inline returnType* functionName()                                                                                \
	{                                                                                                                \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)                                           \
			+ Netvars::GetOffset(clientClass1, table1, name1)                                                        \
			+ Netvars::GetOffset(clientClass2, table2, name2));                                                      \
	}
#define TRIPLE_COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2, clientClass3, table3, name3) \
	inline returnType* functionName()                                                                                                                    \
	{                                                                                                                                                    \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this)                                                                               \
			+ Netvars::GetOffset(clientClass1, table1, name1)                                                                                            \
			+ Netvars::GetOffset(clientClass2, table2, name2)                                                                                            \
			+ Netvars::GetOffset(clientClass3, table3, name3));                                                                                          \
	}

namespace Netvars {
	inline std::map<ClientClass*, std::map<RecvTable*, std::vector<RecvProp*>>> netvars{};

	void DumpNetvars();
	int GetOffset(ClientClassID clientClass, const std::string& table, const std::string& name);
}

#endif
