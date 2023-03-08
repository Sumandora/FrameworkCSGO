#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../ESP.hpp"

void CopyPlayerStateSettings(PlayerStateSettings from, PlayerStateSettings* to)
{
	to->boxName.box = from.boxName.box;
	to->boxName.nametag = from.boxName.nametag;
	to->healthbar = from.healthbar;
	to->weapon = from.weapon;
	to->flashDuration = from.flashDuration;
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

void PlayerStateSettings::SetupGUI(const char* tag)
{
	ImGui::PushID(tag);
	if (ImGui::Button(xorstr_("Copy from")))
		ImGui::OpenPopup(xorstr_("##Copy from"));

	if (ImGui::BeginPopup(xorstr_("##Copy from"))) {
		if (ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(this, Features::Legit::Esp::players.enemy);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(xorstr_("Teammate"))) {
			BuildMenu(this, Features::Legit::Esp::players.teammate);
			ImGui::EndMenu();
		}
		if (ImGui::Selectable(xorstr_("Local"))) {
			CopyPlayerStateSettings(Features::Legit::Esp::players.local, this);
		}
		ImGui::EndPopup();
	}

	boxName.SetupGUI(tag);
	healthbar.SetupGUI(xorstr_("Healthbar"));
	weapon.SetupGUI(xorstr_("Weapon"));
	flashDuration.SetupGUI(xorstr_("Flash duration"));
	ImGui::PopID();
}