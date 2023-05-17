#include "../DebugTab.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../../Netvars.hpp"

void Gui::Construction::Debug::NetvarsTab()
{
	for (const auto& [clientClass, tables] : Netvars::netvars) {
		if (ImGui::TreeNode(clientClass->m_pNetworkName)) {
			ImGui::Text(xorstr_("Class Id: %d"), clientClass->m_ClassID);
			for (const auto& [table, variables] : tables) {
				if (ImGui::TreeNode(table->m_pNetTableName)) {
					ImGui::Text(xorstr_("Main list: %d"), table->m_bInMainList);
					for (const auto& variable : variables) {
						if (ImGui::TreeNode(variable->m_pVarName)) {
							ImGui::Text(xorstr_("[%s][%s][%s] = 0x%x"), clientClass->m_pNetworkName, table->m_pNetTableName, variable->m_pVarName, variable->m_Offset);
							ImGui::Text(xorstr_("Proxy function: %p"), variable->m_ProxyFn);
							ImGui::Text(xorstr_("Data tables proxy function: %p"), variable->m_DataTableProxyFn);
							ImGui::Text(xorstr_("Type: %d"), variable->m_RecvType);
							ImGui::Text(xorstr_("Flags: %d"), variable->m_Flags);
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}