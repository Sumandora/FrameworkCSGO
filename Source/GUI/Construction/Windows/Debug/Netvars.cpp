#include "../Debug.hpp"

#include "imgui.h"

#include "../../../../Netvars.hpp"

void Gui::Windows::netvars()
{
	for (const auto& [clientClass, tables] : Netvars::netvars) {
		if (ImGui::TreeNode(clientClass->m_pNetworkName)) {
			ImGui::Text("Class Id: %d", static_cast<int>(clientClass->m_ClassID));
			for (const auto& [table, variables] : tables) {
				if (ImGui::TreeNode(table->m_pNetTableName)) {
					ImGui::Text("Main list: %d", table->m_bInMainList);
					for (const auto& variable : variables) {
						if (ImGui::TreeNode(variable->m_pVarName)) {
							ImGui::Text("[%s][%s][%s] = 0x%x", clientClass->m_pNetworkName, table->m_pNetTableName, variable->m_pVarName, variable->m_Offset);
							ImGui::Text("Proxy function: %p", variable->m_ProxyFn);
							ImGui::Text("Data tables proxy function: %p", variable->m_DataTableProxyFn);
							ImGui::Text("Type: %d", variable->m_RecvType);
							ImGui::Text("Flags: %d", variable->m_Flags);
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