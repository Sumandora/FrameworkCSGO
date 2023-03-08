#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void WeaponSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon)
{
	char weaponName[256] {};
	if (boxName.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		LocalizeWeaponID(*weapon->WeaponDefinitionIndex(), weaponName);
	}

	boxName.Draw(drawList, rectangle, weaponName);

	int ammoClip = *weapon->Ammo();
	if (ammoClip != -1) { // Does this weapon even have a ammo capacity? (Knifes, Bombs etc...)
		char buf[128];
		sprintf(buf, "%d/%d", ammoClip, *weapon->ReserveAmmoCount());
		ammo.Draw(drawList, rectangle, buf, 1.0f);
	}
}

void WeaponSettings::SetupGUI(const char* tag)
{
	ImGui::PushID(tag);
	boxName.SetupGUI(tag);
	ammo.SetupGUI(xorstr_("Ammo"));
	ImGui::PopID();
}