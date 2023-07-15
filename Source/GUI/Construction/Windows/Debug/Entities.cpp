#include "../Debug.hpp"

#include "../../../../Interfaces.hpp"

#include "imgui.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

static void ShowNetvar(const CBaseEntity* base, const RecvProp* var)
{
	// TODO Use SendPropType here, to determine type:
	const void* ptr = reinterpret_cast<const char*>(base) + var->m_Offset;
	ImGui::Text("Int: %i", *static_cast<const int*>(ptr));
	ImGui::Text("Float: %f", *static_cast<const float*>(ptr));
	ImGui::Text("Vector: (%f,%f,%f)", static_cast<const float*>(ptr)[0], static_cast<const float*>(ptr)[1], static_cast<const float*>(ptr)[2]);
	ImGui::Text("Vector2D: (%f,%f)", static_cast<const float*>(ptr)[0], static_cast<const float*>(ptr)[1]);
	ImGui::Text("String: (%s)", static_cast<const char*>(ptr)); // TODO Make this safe
	ImGui::Text("64-bit Int: (%ld)", *static_cast<const int64_t*>(ptr));
}

void Gui::Windows::Entities()
{
	if (!Interfaces::engine->IsInGame())
		return;
	CClientEntityList* entityList = Interfaces::entityList;
	for (int i = 0; i <= entityList->GetHighestEntityIndex(); i++) {
		CBaseEntity* entity = entityList->GetClientEntity(i);
		if (!entity)
			continue;

		ClientClass* clientClass = entity->GetClientClass();

		if (ImGui::TreeNode(std::to_string(i).c_str(), "%d - %s", i, clientClass->m_pNetworkName)) {
			auto it = Netvars::netvars.find(clientClass);
			if (it != Netvars::netvars.end())
				for (const auto& [table, vars] : it->second)
					if (ImGui::TreeNode(table->m_pNetTableName)) {
						for (const RecvProp* var : vars)
							if (ImGui::TreeNode(var->m_pVarName)) {
								ShowNetvar(entity, var);
								ImGui::TreePop();
							}
						ImGui::TreePop();
					}

			ImGui::TreePop();
		}
	}
}
