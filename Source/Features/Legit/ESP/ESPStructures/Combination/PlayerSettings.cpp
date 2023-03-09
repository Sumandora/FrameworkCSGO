#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

PlayerSettings::PlayerSettings(const char* id)
	: id(id)
	, teammate(PlayerTeamSettings(strdup(xorstr_("Teammate"))))
	, enemy(PlayerTeamSettings(strdup(xorstr_("Enemy"))))
	, local(PlayerStateSettings(strdup(xorstr_("Local"))))
	, spectators(BoxNameSetting(strdup(xorstr_("Spectators"))))
{
}

void PlayerSettings::SetupGUI()
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar(xorstr_("#Player config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Teammate"))) {
			teammate.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Enemy"))) {
			enemy.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Local"))) {
			local.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Spectators"))) {
			spectators.SetupGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerSettings::Serializer, id)
SERIALIZED_STRUCTURE(teammate)
SERIALIZED_STRUCTURE(enemy)
SERIALIZED_STRUCTURE(local)
SERIALIZED_STRUCTURE(spectators)
END_SERIALIZED_STRUCT