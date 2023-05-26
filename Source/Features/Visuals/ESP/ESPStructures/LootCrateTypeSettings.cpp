#include "../ESPStructure.hpp"

#include "../../../../GUI/Elements/Popup.hpp"
#include "../../../General/General.hpp"

static std::map<LootCrateType, const char*> lootCrateNames{
	{ LootCrateType::PISTOL_CASE, strdup(xorstr_("Pistol Case")) },
	{ LootCrateType::LIGHT_CASE, strdup(xorstr_("Light Case")) },
	{ LootCrateType::HEAVY_CASE, strdup(xorstr_("Heavy Case")) },
	{ LootCrateType::EXPLOSIVE_CASE, strdup(xorstr_("Explosive Case")) },
	{ LootCrateType::TOOLS_CASE, strdup(xorstr_("Tools Case")) },
	{ LootCrateType::CASH_DUFFLEBAG, strdup(xorstr_("Cash Dufflebag")) },
	{ LootCrateType::RANDOM_DROP, strdup(xorstr_("Random Drop")) }
};

LootCrateSettings& LootCrateTypeSettings::GetSettings(LootCrateType type) const
{
	switch (type) {
	case LootCrateType::INVALID:
		// Something did go horribly wrong
		Features::General::EventLog::CreateReport(xorstr_("Invalid loot crate found?"));
		__asm("int3");
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
	}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
}
#pragma GCC diagnostic pop

void LootCrateTypeSettings::Draw(ImDrawList* drawList, LootCrate& lootCrate) const
{
	GetSettings(lootCrate.type).Draw(drawList, lootCrate, lootCrateNames[lootCrate.type]);
}

void LootCrateTypeSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar(xorstr_("#Loot crate config selection"), ImGuiTabBarFlags_Reorderable)) {
		for(const auto& [ type, name ] : lootCrateNames) {
			if (ImGui::BeginTabItem(name)) {
				LootCrateSettings& lootCrateSettings = GetSettings(type);

				if (ImGui::Popup(xorstr_("Copy from"), xorstr_("Copy from"))) {
					for(const auto& [ type2, name2 ] : lootCrateNames) {
						if(type == type2)
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

BEGIN_SERIALIZED_STRUCT(LootCrateTypeSettings::Serializer)
SERIALIZED_STRUCTURE(xorstr_("Pistol Case"), pistolCase)
SERIALIZED_STRUCTURE(xorstr_("Light Case"), lightCase)
SERIALIZED_STRUCTURE(xorstr_("Heavy Case"), heavyCase)
SERIALIZED_STRUCTURE(xorstr_("Explosive Case"), explosiveCase)
SERIALIZED_STRUCTURE(xorstr_("Tools Case"), toolsCase)
SERIALIZED_STRUCTURE(xorstr_("Cash Dufflebag"), cashDufflebag)
END_SERIALIZED_STRUCT