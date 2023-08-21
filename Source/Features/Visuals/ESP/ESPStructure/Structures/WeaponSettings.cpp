#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool WeaponSettings::isEnabled() const
{
	return boxName.isEnabled() || ammo.enabled;
}

void WeaponSettings::draw(ImDrawList* drawList, Weapon& weapon) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = weapon.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	const char* weaponName = nullptr;
	if (boxName.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		weaponName = LocalizeWeaponID(weapon.weaponID);
	}

	boxName.draw(drawList, rectangle.value(), weaponName);

	const int ammoClip = weapon.ammo;
	if (ammoClip != -1) { // Does this weapon even have an ammo capacity? (Knifes, Bombs etc...)
		char buf[32];
		sprintf(buf, "%d/%d", ammoClip, weapon.reserveAmmoCount);
		ammo.draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, buf);
	}
}

void WeaponSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	ammo.setupGUI("Ammo");
	ImGui::PopID();
}

SCOPED_SERIALIZER(WeaponSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Ammo", ammo);
}