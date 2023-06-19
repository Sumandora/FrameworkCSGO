#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool PlayerSettings::IsEnabled() const
{
	return teammate.IsEnabled() || enemy.IsEnabled() || local.IsEnabled();
}

void PlayerSettings::Draw(ImDrawList* drawList, Player& player) const
{
	if (!IsEnabled())
		return;

	if (player == EntityCache::localPlayer)
		local.Draw(drawList, player);
	else if (!player.enemy)
		teammate.Draw(drawList, player);
	else
		enemy.Draw(drawList, player);
}

void PlayerSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar(xorstr_("#Player config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Teammate"))) {
			teammate.SetupGUI(xorstr_("Teammate"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Enemy"))) {
			enemy.SetupGUI(xorstr_("Enemy"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Local"))) {
			local.SetupGUI(xorstr_("Local"));
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SCOPED_SERIALIZER(PlayerSettings)
{
	SERIALIZE_STRUCT(xorstr_("Teammate"), teammate);
	SERIALIZE_STRUCT(xorstr_("Enemy"), enemy);
	SERIALIZE_STRUCT(xorstr_("Local"), local);
}