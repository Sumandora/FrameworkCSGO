#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"

#include "../../../../../Interfaces.hpp"

#include "../../ESP.hpp"

#include <memory>

PlayerStateSettings::PlayerStateSettings()
	: flags(FlagsSetting{
		{
			new Money(),
			new Scoped(),
			new PinPulled(),
			new Location(),
			new Objective(),
			new HasDefuser(),
			new HasBomb(),
			new Planting(),
			new Spotted(),
			new Ammo(),
			new Immune(),
			new Walking(),
		} })
{
}

bool PlayerStateSettings::IsEnabled() const
{
	return boxName.IsEnabled() || healthbar.enabled || weapon.enabled || flags.IsEnabled();
}

void PlayerStateSettings::Draw(ImDrawList* drawList, Player& player) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = player.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	char name[MAX_NAME_LEN];
	if (boxName.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(player.index, &info))
			strcpy(name, info.name);
	}
	boxName.Draw(drawList, rectangle.value(), name);

	healthbar.Draw(drawList, rectangle.value(), player.health);

	if (weapon.enabled) { // Don't ask for the weapon, if we don't have to
		if (player.activeWeapon > WeaponID::WEAPON_NONE) { // Also prevent invalids
			const char* localization = LocalizeWeaponID(player.activeWeapon);
			if (localization)
				this->weapon.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, localization);
		}
	}

	flags.Draw(drawList, rectangle->z + boxName.box.GetLineWidth() / 2.0f, rectangle->y, player);
}

void BuildMenu(PlayerStateSettings* playerStateSettings, const PlayerTeamSettings& playerTeamSettings)
{
	if (ImGui::MenuItem(xorstr_("Visible"))) {
		*playerStateSettings = playerTeamSettings.visible;
	}
	if (ImGui::MenuItem(xorstr_("Occluded"))) {
		*playerStateSettings = playerTeamSettings.occluded;
	}
}

void PlayerStateSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);

	if (ImGui::Popup(xorstr_("Copy from"), xorstr_("Copy from"))) {
		if (ImGui::BeginMenu(xorstr_("Teammate"))) {
			BuildMenu(this, esp.players.teammate);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(this, esp.players.enemy);
			ImGui::EndMenu();
		}
		if (ImGui::Selectable(xorstr_("Local"))) {
			*this = esp.players.local;
		}
		ImGui::EndPopup();
	}

	boxName.SetupGUI(id);
	healthbar.SetupGUI(xorstr_("Healthbar"));
	weapon.SetupGUI(xorstr_("Weapon"));
	flags.SetupGUI(xorstr_("Flags"));
	ImGui::PopID();
}

SCOPED_SERIALIZER(PlayerStateSettings)
{
	SERIALIZE_STRUCT(xorstr_("Box name"), boxName);
	SERIALIZE_STRUCT(xorstr_("Healthbar"), healthbar);
	SERIALIZE_STRUCT(xorstr_("Weapon"), weapon);
	SERIALIZE_STRUCT(xorstr_("Flags"), flags);
}