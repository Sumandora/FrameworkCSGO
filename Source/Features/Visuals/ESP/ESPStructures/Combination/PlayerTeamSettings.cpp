#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

PlayerTeamSettings::PlayerTeamSettings()
	: visible(PlayerStateSettings())
	, occluded(PlayerStateSettings())
	, dormant(PlayerStateSettings())
{
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
		if (ImGui::BeginTabItem(xorstr_("Dormant"))) {
			dormant.SetupGUI(xorstr_("Dormant"));
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerTeamSettings::Serializer)
SERIALIZED_STRUCTURE(visible, xorstr_("Visible"))
SERIALIZED_STRUCTURE(occluded, xorstr_("Occluded"))
SERIALIZED_STRUCTURE(dormant, xorstr_("Dormant"))
END_SERIALIZED_STRUCT