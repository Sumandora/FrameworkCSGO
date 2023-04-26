#include "Netvars.hpp"

#include <cstdio>
#include <cstring>

#include "Interfaces.hpp"
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

RecvProp* Netvars::GetOffset(ClientClassID clientClass, const char* table, const char* name)
{
	for (const auto& [key, value] : netvars) {
		if (key->m_ClassID == static_cast<int>(clientClass))
			for (const auto& [key2, value2] : value) {
				if (strcmp(key2->m_pNetTableName, table) == 0)
					for (const auto& variable : value2) {
						if (strcmp(name, variable->m_pVarName) == 0)
							return variable;
					}
			}
	}

	printf(xorstr_("Couldn't find netvar %s in %s\n"), name, table);
	return nullptr;
}
