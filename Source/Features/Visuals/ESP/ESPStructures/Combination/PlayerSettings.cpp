#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

PlayerSettings::PlayerSettings()
	: teammate(PlayerTeamSettings())
	, enemy(PlayerTeamSettings())
	, local(PlayerStateSettings())
	, spectators(BoxNameSetting())
{
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
		if (ImGui::BeginTabItem(xorstr_("Spectators"))) {
			spectators.SetupGUI(xorstr_("Spectators"));
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerSettings::Serializer)
SERIALIZED_STRUCTURE(teammate, xorstr_("Teammate"))
SERIALIZED_STRUCTURE(enemy, xorstr_("Enemy"))
SERIALIZED_STRUCTURE(local, xorstr_("Local"))
SERIALIZED_STRUCTURE(spectators, xorstr_("Spectators"))
END_SERIALIZED_STRUCT