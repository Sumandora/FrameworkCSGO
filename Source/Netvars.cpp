#include "Netvars.hpp"

#include <cstdio>
#include <cstring>
#include <map>

#include "Interfaces.hpp"
#include "imgui.h"
#include "xorstr.hpp"

#include "Utils/VMT.hpp"

#include "SDK/Netvars/ClientClass.hpp"
#include "SDK/Netvars/RecvProp.hpp"
#include "SDK/Netvars/RecvTable.hpp"

std::map<char*, std::map<char*, int>> netvars;

void								  ReadTable(RecvTable* recvTable) {
	 for (int i = 0; i < recvTable->m_nProps; i++) {
		 RecvProp* prop = &recvTable->m_pProps[i];
		 if (!prop)
			 continue;
		 if (strcmp(prop->m_pVarName, xorstr_("baseclass")) == 0)
			 continue;

		 if (!netvars.contains(recvTable->m_pNetTableName)) // It should never already exist, but I don't trust Source
			 netvars[recvTable->m_pNetTableName] = {};

		 if (!netvars[recvTable->m_pNetTableName].contains(prop->m_pVarName))
			 netvars[recvTable->m_pNetTableName][prop->m_pVarName] = prop->m_Offset;

		 if (prop->m_pDataTable && strcmp(prop->m_pDataTable->m_pNetTableName, prop->m_pVarName) != 0) // sometimes there are tables, which have var names. They are always second; skip them
			 ReadTable(prop->m_pDataTable);
	 }
}

void Netvars::DumpNetvars() {
	/**
	 * GetAllClasses assembly:
	 *	lea	0x23c9fc1(%rip),%rax		# 0x7f44ff24df08
	 *	push %rbp
	 *	mov	%rsp,%rbp
	 *	pop	%rbp
	 *	mov	(%rax),%rax
	 * 	ret
	 */

	void*		 getAllClasses	 = Utils::GetVTable(Interfaces::baseClient)[8];
	char*		 relativeAddress = static_cast<char*>(getAllClasses) + 3;
	ClientClass* rootClass		 = *reinterpret_cast<ClientClass**>(Memory::RelativeToAbsolute(relativeAddress));

	for (ClientClass* cClass = rootClass; cClass != nullptr; cClass = cClass->m_pNext) {
		RecvTable* table = cClass->m_pRecvTable;
		ReadTable(table);
	}
}

int Netvars::GetOffset(const char* table, const char* name) {
	for (const auto& [key, value] : netvars) {
		if (strcmp(table, key) == 0)
			for (const auto& [key2, value2] : value) {
				if (strcmp(name, key2) == 0)
					return value2;
			}
	}
	printf(xorstr_("Couldn't find netvar %s in %s\n"), name, table);
	return 0;
}

void Netvars::BuildGUI() {
	static char searchBar[128] = "";
	ImGui::InputText(xorstr_("Search bar"), searchBar, 128);

	for (const auto& [key, value] : netvars) {
		if (searchBar[0] == '\0' || strncmp(searchBar, key, strlen(searchBar)) == 0) {
			if (ImGui::TreeNode(key)) {
				for (const auto& [key2, value2] : value) {
					if (ImGui::TreeNode(key2)) {
						ImGui::Text(xorstr_("[%s][%s] = 0x%x\n"), key, key2, value2);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
}
