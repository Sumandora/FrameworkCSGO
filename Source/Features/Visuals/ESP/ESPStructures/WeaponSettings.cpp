#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void WeaponSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, const Weapon& weapon) const
{
	const char* weaponName = nullptr;
	if (boxName.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		weaponName = LocalizeWeaponID(weapon.weaponID);
	}

	boxName.Draw(drawList, rectangle, weaponName);

	const int ammoClip = weapon.ammo;
	if (ammoClip != -1) { // Does this weapon even have an ammo capacity? (Knifes, Bombs etc...)
		char buf[128];
		sprintf(buf, xorstr_("%d/%d"), ammoClip, weapon.reserveAmmoCount);
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
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Ammo"), ammo)
END_SERIALIZED_STRUCT