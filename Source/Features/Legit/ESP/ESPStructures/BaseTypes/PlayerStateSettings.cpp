#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../Legit.hpp"

PlayerStateSettings::PlayerStateSettings(const char* id)
	: id(id)
	, boxName(BoxNameSetting(id))
	, healthbar(HealthbarSettings(strdup(xorstr_("Healthbar"))))
	, weapon(TextSetting(strdup(xorstr_("Weapon"))))
	, flashDuration(TextSetting(strdup(xorstr_("Flash duration"))))
{
}

void CopyPlayerStateSettings(PlayerStateSettings from, PlayerStateSettings* to)
{
	to->boxName.Copy(from.boxName);
	to->healthbar.Copy(from.healthbar);
	to->weapon.Copy(from.weapon);
	to->flashDuration.Copy(from.flashDuration);
}

void BuildMenu(PlayerStateSettings* playerStateSettings, PlayerTeamSettings playerTeamSettings)
{
	if (ImGui::MenuItem(xorstr_("Visible"))) {
		CopyPlayerStateSettings(playerTeamSettings.visible, playerStateSettings);
	}
	if (ImGui::MenuItem(xorstr_("Occluded"))) {
		CopyPlayerStateSettings(playerTeamSettings.occluded, playerStateSettings);
	}
	if (ImGui::MenuItem(xorstr_("Dormant"))) {
		CopyPlayerStateSettings(playerTeamSettings.dormant, playerStateSettings);
	}
}

void PlayerStateSettings::SetupGUI()
{
	ImGui::PushID(id);
	if (ImGui::Button(xorstr_("Copy from")))
		ImGui::OpenPopup(xorstr_("##Copy from"));

	if (ImGui::BeginPopup(xorstr_("##Copy from"))) {
		if (ImGui::BeginMenu(xorstr_("Teammate"))) {
			BuildMenu(this, Features::Legit::Esp::players.teammate);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(this, Features::Legit::Esp::players.enemy);
			ImGui::EndMenu();
		}
		if (ImGui::Selectable(xorstr_("Local"))) {
			CopyPlayerStateSettings(Features::Legit::Esp::players.local, this);
		}
		ImGui::EndPopup();
	}

	boxName.SetupGUI();
	healthbar.SetupGUI();
	weapon.SetupGUI();
	flashDuration.SetupGUI();
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerStateSettings::Serializer, id)
SERIALIZED_STRUCTURE(boxName)
SERIALIZED_STRUCTURE(healthbar)
SERIALIZED_STRUCTURE(weapon)
SERIALIZED_STRUCTURE(flashDuration)
END_SERIALIZED_STRUCT