#include "../ESPStructure.hpp"

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
	if (ImGui::BeginTabBar("#Player config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Teammate")) {
			teammate.SetupGUI("Teammate");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Enemy")) {
			enemy.SetupGUI("Enemy");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Local")) {
			local.SetupGUI("Local");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SCOPED_SERIALIZER(PlayerSettings)
{
	SERIALIZE_STRUCT("Teammate", teammate);
	SERIALIZE_STRUCT("Enemy", enemy);
	SERIALIZE_STRUCT("Local", local);
}