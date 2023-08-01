#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"

#include "../../../../General/EventLog.hpp"

static std::map<LootCrateType, const char*> lootCrateNames{
	{ LootCrateType::PISTOL_CASE, "Pistol Case" },
	{ LootCrateType::LIGHT_CASE, "Light Case" },
	{ LootCrateType::HEAVY_CASE, "Heavy Case" },
	{ LootCrateType::EXPLOSIVE_CASE, "Explosive Case" },
	{ LootCrateType::TOOLS_CASE, "Tools Case" },
	{ LootCrateType::CASH_DUFFLEBAG, "Cash Dufflebag" },
	{ LootCrateType::RANDOM_DROP, "Random Drop" }
};

bool LootCrateTypeSettings::IsEnabled() const
{
	return pistolCase.IsEnabled() || lightCase.IsEnabled() || heavyCase.IsEnabled() || explosiveCase.IsEnabled() || toolsCase.IsEnabled() || cashDufflebag.IsEnabled();
}

LootCrateSettings& LootCrateTypeSettings::GetSettings(LootCrateType type) const
{
	switch (type) {
	case LootCrateType::PISTOL_CASE:
		return pistolCase;
	case LootCrateType::LIGHT_CASE:
		return lightCase;
	case LootCrateType::HEAVY_CASE:
		return heavyCase;
	case LootCrateType::EXPLOSIVE_CASE:
		return explosiveCase;
	case LootCrateType::TOOLS_CASE:
		return toolsCase;
	case LootCrateType::CASH_DUFFLEBAG:
		return cashDufflebag;
	case LootCrateType::RANDOM_DROP:
		return randomDrop;
	case LootCrateType::INVALID:
	default:
		__builtin_unreachable();
	}
}

void LootCrateTypeSettings::Draw(ImDrawList* drawList, LootCrate& lootCrate) const
{
	GetSettings(lootCrate.type).Draw(drawList, lootCrate, lootCrateNames[lootCrate.type]);
}

void LootCrateTypeSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar("#Loot crate config selection", ImGuiTabBarFlags_Reorderable)) {
		for (const auto& [type, name] : lootCrateNames) {
			if (ImGui::BeginTabItem(name)) {
				LootCrateSettings& lootCrateSettings = GetSettings(type);

				if (ImGui::Popup("Copy from", "Copy from")) {
					for (const auto& [type2, name2] : lootCrateNames) {
						if (type == type2)
							continue;

						if (ImGui::Selectable(name2)) {
							lootCrateSettings = GetSettings(type2);
						}
					}
					ImGui::EndPopup();
				}

				lootCrateSettings.SetupGUI(name);
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SCOPED_SERIALIZER(LootCrateTypeSettings)
{
	SERIALIZE_STRUCT("Pistol Case", pistolCase);
	SERIALIZE_STRUCT("Light Case", lightCase);
	SERIALIZE_STRUCT("Heavy Case", heavyCase);
	SERIALIZE_STRUCT("Explosive Case", explosiveCase);
	SERIALIZE_STRUCT("Tools Case", toolsCase);
	SERIALIZE_STRUCT("Cash Dufflebag", cashDufflebag);
}