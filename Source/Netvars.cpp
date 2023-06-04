#include "Netvars.hpp"

#include <cstdio>
#include <cstring>
#include <tuple>
#include <unordered_map>

#include "Interfaces.hpp"
#include "SDK/ClientClassIDs.hpp"
#include "SDK/Netvars/RecvProp.hpp"
#include "xorstr.hpp"

#include "Utils/VMT.hpp"

void ReadTable(ClientClass* clientClass, RecvTable* recvTable)
{
	for (int i = 0; i < recvTable->m_nProps; i++) {
		RecvProp* prop = &recvTable->m_pProps[i];
		if (!prop)
			continue;
		if (strcmp(prop->m_pVarName, xorstr_("baseclass")) == 0)
			continue;

		Netvars::netvars[clientClass][recvTable].emplace_back(prop);

		if (prop->m_pDataTable && strcmp(prop->m_pDataTable->m_pNetTableName, prop->m_pVarName) != 0) // sometimes there are tables, which have var names. They are always second; skip them
			ReadTable(clientClass, prop->m_pDataTable);
	}
}

void Netvars::DumpNetvars()
{
	/**
	 * GetAllClasses assembly:
	 *	lea	0x23c9fc1(%rip),%rax		# 0x7f44ff24df08
	 *	push %rbp
	 *	mov	%rsp,%rbp
	 *	pop	%rbp
	 *	mov	(%rax),%rax
	 * 	ret
	 */

	void* getAllClasses = Utils::GetVTable(Interfaces::baseClient)[8];
	char* relativeAddress = reinterpret_cast<char*>(getAllClasses) + 3;
	ClientClass* rootClass = *reinterpret_cast<ClientClass**>(Memory::RelativeToAbsolute(relativeAddress));

	for (ClientClass* cClass = rootClass; cClass != nullptr; cClass = cClass->m_pNext) {
		RecvTable* table = cClass->m_pRecvTable;
		ReadTable(cClass, table);
	}
}

RecvProp* LookupRecvProp(ClientClassID clientClassID, const std::string& tableName, const std::string& variableName)
{
	for (const auto& [clientClass, tables] : Netvars::netvars)
		if (clientClass->m_ClassID == clientClassID)
			for (const auto& [table, variables] : tables)
				if (table->m_pNetTableName == tableName)
					for (const auto& variable : variables)
						if (variable->m_pVarName == variableName)
							return variable;
	__builtin_unreachable();
}

int Netvars::GetOffset(ClientClassID clientClass, const std::string& table, const std::string& name)
{
	static std::map<std::tuple<ClientClassID, std::string, std::string>, int> cache;

	auto tuple = std::make_tuple(clientClass, table, name);

	if (!cache.contains(tuple))
		cache[tuple] = LookupRecvProp(clientClass, table, name)->m_Offset;

	return cache[tuple];
}
