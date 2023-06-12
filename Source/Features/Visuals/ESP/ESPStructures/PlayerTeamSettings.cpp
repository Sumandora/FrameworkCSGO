#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

#include "../../Visuals.hpp"

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
	if (ImGui::BeginTabBar(xorstr_("#Player team config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Visible"))) {
			visible.SetupGUI(xorstr_("Visible"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Occluded"))) {
			occluded.SetupGUI(xorstr_("Occluded"));
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerTeamSettings::Serializer)
SERIALIZED_STRUCTURE(xorstr_("Visible"), visible)
SERIALIZED_STRUCTURE(xorstr_("Occluded"), occluded)
END_SERIALIZED_STRUCT