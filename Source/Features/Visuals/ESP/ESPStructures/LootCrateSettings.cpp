#include "../ESPStructure.hpp"

#include "../../../../GUI/Elements/Popup.hpp"
#include "../../../General/General.hpp"

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
	healthbar.SetupGUI(xorstr_("Healthbar"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(LootCrateSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Healthbar"), healthbar)
END_SERIALIZED_STRUCT