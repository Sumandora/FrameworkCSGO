#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool PlayerTeamSettings::IsEnabled() const
{
	return visible.IsEnabled() || occluded.IsEnabled();
}

void PlayerTeamSettings::Draw(ImDrawList* drawList, Player& player) const
{
	if (!IsEnabled())
		return;

	if (player.visible)
		visible.Draw(drawList, player);
	else
		occluded.Draw(drawList, player);
}

void PlayerTeamSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar("#Player team config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Visible")) {
			visible.SetupGUI("Visible");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Occluded")) {
			occluded.SetupGUI("Occluded");
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