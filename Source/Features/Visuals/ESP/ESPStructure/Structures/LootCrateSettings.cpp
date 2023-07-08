#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"

bool LootCrateSettings::IsEnabled() const
{
	return boxName.IsEnabled() || healthbar.enabled;
}

void LootCrateSettings::Draw(ImDrawList* drawList, LootCrate& lootCrate, const char* name) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = lootCrate.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), name);
	healthbar.Draw(drawList, rectangle.value(), lootCrate.health, lootCrate.maxHealth);
}

void LootCrateSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	healthbar.SetupGUI("Healthbar");
	ImGui::PopID();
}

SCOPED_SERIALIZER(LootCrateSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Healthbar", healthbar);
}