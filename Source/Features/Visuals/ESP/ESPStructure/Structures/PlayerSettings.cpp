#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool PlayerSettings::isEnabled() const
{
	return teammate.isEnabled() || enemy.isEnabled() || local.isEnabled();
}

void PlayerSettings::draw(ImDrawList* drawList, Player& player) const
{
	if (!isEnabled())
		return;

	if (EntityCache::localPlayer.has_value() && player == EntityCache::localPlayer)
		local.draw(drawList, player);
	else if (!player.enemy)
		teammate.draw(drawList, player);
	else if (player.enemy)
		enemy.draw(drawList, player);
}

void PlayerSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar("#Player config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Teammate")) {
			teammate.setupGUI("Teammate");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Enemy")) {
			enemy.setupGUI("Enemy");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Local")) {
			local.setupGUI("Local");
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