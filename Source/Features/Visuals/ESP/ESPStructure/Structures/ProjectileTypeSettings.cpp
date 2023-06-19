#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../General/EventLog.hpp"

static std::map<ProjectileType, const char*> projectileNames{
	{ ProjectileType::BREACH_CHARGE, strdup(xorstr_("Breach charge")) },
	{ ProjectileType::BUMP_MINE, strdup(xorstr_("Bump Mine")) },
	{ ProjectileType::DECOY, strdup(xorstr_("Decoy")) },
	{ ProjectileType::MOLOTOV, strdup(xorstr_("Molotov")) },
	{ ProjectileType::SENSOR_GRENADE, strdup(xorstr_("Sensor Grenade")) },
	{ ProjectileType::SMOKE_GRENADE, strdup(xorstr_("Smoke Grenade")) },
	{ ProjectileType::SNOWBALL, strdup(xorstr_("Snowball")) },
	{ ProjectileType::HIGH_EXPLOSIVE_GRENADE, strdup(xorstr_("High Explosive Grenade")) },
	{ ProjectileType::FLASHBANG, strdup(xorstr_("Flashbang")) },
};

bool ProjectileTypeSettings::IsEnabled() const
{
	return breachCharge.IsEnabled() || bumpMine.IsEnabled() || decoy.IsEnabled() || molotov.IsEnabled() || sensorGrenade.IsEnabled() || smokeGrenade.IsEnabled() || snowball.IsEnabled() || highExplosiveGrenade.IsEnabled() || flashbang.IsEnabled();
}

ProjectileSettings& ProjectileTypeSettings::GetSettings(ProjectileType type) const
{
	switch (type) {
	case ProjectileType::INVALID:
		// Something did go horribly wrong
		eventLog.CreateReport(xorstr_("Invalid projectile found?"));
		__asm("int3");
		__builtin_unreachable();
	case ProjectileType::BREACH_CHARGE:
		return breachCharge;
	case ProjectileType::BUMP_MINE:
		return bumpMine;
	case ProjectileType::DECOY:
		return decoy;
	case ProjectileType::MOLOTOV:
		return molotov;
	case ProjectileType::SENSOR_GRENADE:
		return sensorGrenade;
	case ProjectileType::SMOKE_GRENADE:
		return smokeGrenade;
	case ProjectileType::SNOWBALL:
		return snowball;
	case ProjectileType::HIGH_EXPLOSIVE_GRENADE:
		return highExplosiveGrenade;
	case ProjectileType::FLASHBANG:
		return flashbang;
	}
}

void ProjectileTypeSettings::Draw(ImDrawList* drawList, Projectile& projectile) const
{
	GetSettings(projectile.type).Draw(drawList, projectile, projectileNames[projectile.type]);
}

void ProjectileTypeSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar(xorstr_("#Projectile config selection"), ImGuiTabBarFlags_Reorderable)) {
		for (const auto& [type, name] : projectileNames) {
			if (ImGui::BeginTabItem(name)) {
				ProjectileSettings& projectileSettings = GetSettings(type);

				if (ImGui::Popup(xorstr_("Copy from"), xorstr_("Copy from"))) {
					for (const auto& [type2, name2] : projectileNames) {
						if (type == type2)
							continue;

						if (ImGui::Selectable(name2)) {
							projectileSettings = GetSettings(type2);
						}
					}
					ImGui::EndPopup();
				}

				projectileSettings.SetupGUI(name);
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

SCOPED_SERIALIZER(ProjectileTypeSettings)
{
	SERIALIZE_STRUCT(xorstr_("Breach charge"), breachCharge);
	SERIALIZE_STRUCT(xorstr_("Bump Mine"), bumpMine);
	SERIALIZE_STRUCT(xorstr_("Decoy"), decoy);
	SERIALIZE_STRUCT(xorstr_("Molotov"), molotov);
	SERIALIZE_STRUCT(xorstr_("Sensor Grenade"), sensorGrenade);
	SERIALIZE_STRUCT(xorstr_("Smoke Grenade"), smokeGrenade);
	SERIALIZE_STRUCT(xorstr_("Snowball"), snowball);
	SERIALIZE_STRUCT(xorstr_("High Explosive Grenade"), highExplosiveGrenade);
	SERIALIZE_STRUCT(xorstr_("Flashbang"), flashbang);
}