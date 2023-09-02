#include "Netvars.hpp"

#include <cstring>
#include <tuple>

#include "BCRL.hpp"
#include "Interfaces.hpp"
#include "SDK/ClientClassIDs.hpp"
#include "SDK/Netvars/RecvProp.hpp"

static void readTable(ClientClass* clientClass, RecvTable* recvTable)
{
	for (int i = 0; i < recvTable->m_nProps; i++) {
		RecvProp* prop = &recvTable->m_pProps[i];
		if (!prop)
			continue;
		if (strcmp(prop->m_pVarName, "baseclass") == 0)
			continue;

		Netvars::netvars[clientClass][recvTable].emplace_back(prop);

		if (prop->m_pDataTable && strcmp(prop->m_pDataTable->m_pNetTableName, prop->m_pVarName) != 0) // sometimes there are tables, which have var names. They are always second; skip them
			readTable(clientClass, prop->m_pDataTable);
	}
}

void Netvars::dumpNetvars()
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

	ClientClass* rootClass = static_cast<ClientClass*>(BCRL::Session::arrayPointer(Interfaces::baseClient, 8)
														   .add(3)
														   .relativeToAbsolute()
														   .dereference()
														   .getPointer()
														   .value());

	for (ClientClass* cClass = rootClass; cClass != nullptr; cClass = cClass->m_pNext) {
		RecvTable* table = cClass->m_pRecvTable;
		readTable(cClass, table);
	}
}

RecvProp* Netvars::getOffset(ClientClassID clientClassID, const std::string& tableName, const std::string& variableName)
{
	for (const auto& [clientClass, tables] : Netvars::netvars)
		if (clientClass->m_ClassID == clientClassID)
			for (const auto& [table, variables] : tables)
				if (table->m_pNetTableName == tableName)
					for (const auto& variable : variables)
						if (variable->m_pVarName == variableName)
							return variable;

	__asm("int3");
	__builtin_unreachable();
}
