#include "../EntityCache.hpp"

static std::map<const char*, LootCrateType> lootCrateMap{
	{ "case_pistol.mdl", LootCrateType::PISTOL_CASE },
	{ "case_light_weapon.mdl", LootCrateType::LIGHT_CASE },
	{ "case_heavy_weapon.mdl", LootCrateType::HEAVY_CASE },
	{ "case_explosive.mdl", LootCrateType::EXPLOSIVE_CASE },
	{ "case_tools.mdl", LootCrateType::TOOLS_CASE },
	{ "dufflebag.mdl", LootCrateType::CASH_DUFFLEBAG },
	{ "case_random_drop.mdl", LootCrateType::RANDOM_DROP }
};

void LootCrate::Update(CPhysPropLootCrate* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	health = *entity->Health();
	maxHealth = *entity->MaxHealth();

	const char* modelName = entity->GetModel()->szPathName;
	for (const auto& [substring, lootCrateType] : lootCrateMap) {
		if (strstr(modelName, substring)) {
			type = lootCrateType;
			return;
		}
	}

	type = LootCrateType::INVALID;
}
