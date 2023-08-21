#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool PlayerTeamSettings::isEnabled() const
{
	return visible.isEnabled() || occluded.isEnabled();
}

void PlayerTeamSettings::draw(ImDrawList* drawList, Player& player) const
{
	if (!isEnabled())
		return;

	if (player.visible)
		visible.draw(drawList, player);
	else
		occluded.draw(drawList, player);
}

void PlayerTeamSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar("#Player team config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Visible")) {
			visible.setupGUI("Visible");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Occluded")) {
			occluded.setupGUI("Occluded");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SCOPED_SERIALIZER(PlayerTeamSettings)
{
	SERIALIZE_STRUCT("Visible", visible);
	SERIALIZE_STRUCT("Occluded", occluded);
}