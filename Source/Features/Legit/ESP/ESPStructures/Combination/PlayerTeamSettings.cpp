#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

PlayerTeamSettings::PlayerTeamSettings(const char* id)
	: id(id)
	, visible(PlayerStateSettings(strdup(xorstr_("Visible"))))
	, occluded(PlayerStateSettings(strdup(xorstr_("Occluded"))))
	, dormant(PlayerStateSettings(strdup(xorstr_("Dormant"))))
{
}

void PlayerTeamSettings::SetupGUI()
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar(xorstr_("#Player team config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Visible"))) {
			visible.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Occluded"))) {
			occluded.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Dormant"))) {
			dormant.SetupGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerTeamSettings::Serializer, id)
SERIALIZED_STRUCTURE(visible)
SERIALIZED_STRUCTURE(occluded)
SERIALIZED_STRUCTURE(dormant)
END_SERIALIZED_STRUCT