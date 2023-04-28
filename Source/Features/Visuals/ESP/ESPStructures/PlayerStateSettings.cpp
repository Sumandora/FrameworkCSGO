#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../GUI/Elements/Popup.hpp"

#include "../../../../Interfaces.hpp"

#include "../../../Visuals.hpp"

PlayerStateSettings::PlayerStateSettings()
	: boxName(BoxNameSetting())
	, healthbar(HealthbarSettings())
	, weapon(TextSetting())
	, flashDuration(TextSetting())
{
}

void PlayerStateSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player) const
{
	char name[128];
	if (boxName.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		PlayerInfo info {};
		Interfaces::engine->GetPlayerInfo(player->entindex(), &info);
		strcpy(name, info.name);
	}
	boxName.Draw(drawList, rectangle, name);

	healthbar.Draw(drawList, rectangle, *player->Health());
	if (weapon.enabled) { // Don't ask for the weapon, if we don't have to
		auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(
			player->ActiveWeapon()));
		if (weapon) {
			const WeaponID weaponID = *weapon->WeaponDefinitionIndex();
			if (weaponID > WeaponID::WEAPON_NONE) { // Also prevent invalids
				const char* localization = LocalizeWeaponID(weaponID);
				if(localization)
					this->weapon.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.w, true, localization);
			}
		}
	}

	const float flashDuration = player->GetFlashAlpha();
	if (flashDuration > 0.0) {
		this->flashDuration.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.y + (rectangle.w - rectangle.y) * 0.5f - this->flashDuration.GetLineHeight() / 2.0f, true, std::to_string((int)flashDuration).c_str());
	}
}

void BuildMenu(PlayerStateSettings* playerStateSettings, const PlayerTeamSettings& playerTeamSettings)
{
	if (ImGui::MenuItem(xorstr_("Visible"))) {
		*playerStateSettings = playerTeamSettings.visible;
	}
	if (ImGui::MenuItem(xorstr_("Occluded"))) {
		*playerStateSettings = playerTeamSettings.occluded;
	}
	if (ImGui::MenuItem(xorstr_("Dormant"))) {
		*playerStateSettings = playerTeamSettings.dormant;
	}
}

void PlayerStateSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);

	if (ImGui::Popup(xorstr_("Copy from"), xorstr_("Copy from"))) {
		if (ImGui::BeginMenu(xorstr_("Teammate"))) {
			BuildMenu(this, Features::Visuals::Esp::players.teammate);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(this, Features::Visuals::Esp::players.enemy);
			ImGui::EndMenu();
		}
		if (ImGui::Selectable(xorstr_("Local"))) {
			*this = Features::Visuals::Esp::players.local;
		}
		ImGui::EndPopup();
	}

	boxName.SetupGUI(id);
	healthbar.SetupGUI(xorstr_("Healthbar"));
	weapon.SetupGUI(xorstr_("Weapon"));
	flashDuration.SetupGUI(xorstr_("Flash duration"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlayerStateSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Healthbar"), healthbar)
SERIALIZED_STRUCTURE(xorstr_("Weapon"), weapon)
SERIALIZED_STRUCTURE(xorstr_("Flash duration"), flashDuration)
END_SERIALIZED_STRUCT