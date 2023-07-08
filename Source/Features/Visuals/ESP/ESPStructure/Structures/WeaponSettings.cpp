#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool WeaponSettings::IsEnabled() const
{
	return boxName.IsEnabled() || ammo.enabled;
}

void WeaponSettings::Draw(ImDrawList* drawList, Weapon& weapon) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = weapon.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	const char* weaponName = nullptr;
	if (boxName.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		weaponName = LocalizeWeaponID(weapon.weaponID);
	}

	boxName.Draw(drawList, rectangle.value(), weaponName);

	const int ammoClip = weapon.ammo;
	if (ammoClip != -1) { // Does this weapon even have an ammo capacity? (Knifes, Bombs etc...)
		char buf[32];
		sprintf(buf, "%d/%d", ammoClip, weapon.reserveAmmoCount);
		ammo.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, buf);
	}
}

void WeaponSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	ammo.SetupGUI("Ammo");
	ImGui::PopID();
}

SCOPED_SERIALIZER(WeaponSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Ammo", ammo);
}