#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

WeaponSettings::WeaponSettings(const char* id)
	: id(id)
	, boxName(BoxNameSetting(id))
	, ammo(TextSetting(strdup(xorstr_("Ammo"))))
{
}

void WeaponSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon) const
{
	char weaponName[256] {};
	if (boxName.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		LocalizeWeaponID(*weapon->WeaponDefinitionIndex(), weaponName);
	}

	boxName.Draw(drawList, rectangle, weaponName);

	const int ammoClip = *weapon->Ammo();
	if (ammoClip != -1) { // Does this weapon even have an ammo capacity? (Knifes, Bombs etc...)
		char buf[128];
		sprintf(buf, xorstr_("%d/%d"), ammoClip, *weapon->ReserveAmmoCount());
		ammo.Draw(drawList, rectangle, buf, 1.0F);
	}
}

void WeaponSettings::SetupGUI()
{
	ImGui::PushID(id);
	boxName.SetupGUI();
	ammo.SetupGUI();
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(WeaponSettings::Serializer, id)
SERIALIZED_STRUCTURE(boxName)
SERIALIZED_STRUCTURE(ammo)
END_SERIALIZED_STRUCT