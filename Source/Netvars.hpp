#ifndef NETVARS
#define NETVARS

#include <map>
#include <string>
#include <vector>

#include "SDK/ClientClassIDs.hpp"

#include "SDK/Netvars/ClientClass.hpp"
#include "SDK/Netvars/RecvTable.hpp"

#define NETVAR_FUNCTION(returnType, functionName, clientClass, table, name)           \
	inline returnType* functionName()                                                 \
	{                                                                                 \
		static int offset = Netvars::getOffset(clientClass, table, name)->m_Offset;   \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this) + offset); \
	}
#define COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2) \
	inline returnType* functionName()                                                                                \
	{                                                                                                                \
		static int offset1 = Netvars::getOffset(clientClass1, table1, name1)->m_Offset;                              \
		static int offset2 = Netvars::getOffset(clientClass2, table2, name2)->m_Offset;                              \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this) + offset1 + offset2);                     \
	}
#define TRIPLE_COMBINED_NETVAR_FUNCTION(returnType, functionName, clientClass1, table1, name1, clientClass2, table2, name2, clientClass3, table3, name3) \
	inline returnType* functionName()                                                                                                                    \
	{                                                                                                                                                    \
		static int offset1 = Netvars::getOffset(clientClass1, table1, name1)->m_Offset;                                                                  \
		static int offset2 = Netvars::getOffset(clientClass2, table2, name2)->m_Offset;                                                                  \
		static int offset3 = Netvars::getOffset(clientClass3, table3, name3)->m_Offset;                                                                  \
		return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this) + offset1 + offset2 + offset3);                                               \
	}

namespace Netvars {
	inline std::map<ClientClass*, std::map<RecvTable*, std::vector<RecvProp*>>> netvars{};

	void dumpNetvars();
	RecvProp* getOffset(ClientClassID clientClassID, const std::string& tableName, const std::string& variableName);
}

#endif
