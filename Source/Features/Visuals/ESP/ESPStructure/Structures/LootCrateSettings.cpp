#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"

bool LootCrateSettings::isEnabled() const
{
	return boxName.isEnabled() || healthbar.enabled;
}

void LootCrateSettings::draw(ImDrawList* drawList, LootCrate& lootCrate, const char* name) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = lootCrate.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), name);
	healthbar.draw(drawList, rectangle.value(), lootCrate.health, lootCrate.maxHealth);
}

void LootCrateSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	healthbar.setupGUI("Healthbar");
	ImGui::PopID();
}

SCOPED_SERIALIZER(LootCrateSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Healthbar", healthbar);
}