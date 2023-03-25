#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Interfaces.hpp"
#include "../../../../Visuals.hpp"

PlayerStateSettings::PlayerStateSettings(const char* id)
	: id(id)
	, boxName(BoxNameSetting(id))
	, healthbar(HealthbarSettings(strdup(xorstr_("Healthbar"))))
	, weapon(TextSetting(strdup(xorstr_("Weapon"))))
	, flashDuration(TextSetting(strdup(xorstr_("Flash duration"))))
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
				char weaponName[256];
				LocalizeWeaponID(weaponID, weaponName);
				this->weapon.Draw(drawList,rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.w, true, weaponName);
			}
		}
	}

	const float flashDuration = *reinterpret_cast<float*>(reinterpret_cast<char*>(player->FlashMaxAlpha()) - 0x8);
	if (flashDuration > 0.0) {
		this->flashDuration.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.y + (rectangle.w - rectangle.y) * 0.5f - this->flashDuration.GetLineHeight() / 2.0f, true, std::to_string((int)flashDuration).c_str());
	}
}

bool PlayerStateSettings::operator==(const PlayerStateSettings& other) const
{
	// clang-format off
	return
		boxName == other.boxName &&
		healthbar == other.healthbar &&
		weapon == other.weapon &&
		flashDuration == other.flashDuration;
	// clang-format on
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
			BuildMenu(this, Features::Visuals::Esp::players.teammate);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(this, Features::Visuals::Esp::players.enemy);
			ImGui::EndMenu();
		}
		if (ImGui::Selectable(xorstr_("Local"))) {
			CopyPlayerStateSettings(Features::Visuals::Esp::players.local, this);
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