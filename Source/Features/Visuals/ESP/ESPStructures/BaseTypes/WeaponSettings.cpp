#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

WeaponSettings::WeaponSettings()
	: boxName(BoxNameSetting())
	, ammo(TextSetting())
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
		ammo.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.w, true, buf);
	}
}

void WeaponSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	ammo.SetupGUI(xorstr_("Ammo"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(WeaponSettings::Serializer)
SERIALIZED_STRUCTURE(boxName, name)
SERIALIZED_STRUCTURE(ammo, xorstr_("Ammo"))
END_SERIALIZED_STRUCT