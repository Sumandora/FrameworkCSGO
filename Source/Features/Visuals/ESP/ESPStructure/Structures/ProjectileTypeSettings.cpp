#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../General/EventLog.hpp"

static std::map<ProjectileType, const char*> projectileNames{
	{ ProjectileType::BREACH_CHARGE, "Breach charge" },
	{ ProjectileType::BUMP_MINE, "Bump Mine" },
	{ ProjectileType::DECOY, "Decoy" },
	{ ProjectileType::MOLOTOV, "Molotov" },
	{ ProjectileType::SENSOR_GRENADE, "Sensor Grenade" },
	{ ProjectileType::SMOKE_GRENADE, "Smoke Grenade" },
	{ ProjectileType::SNOWBALL, "Snowball" },
	{ ProjectileType::HIGH_EXPLOSIVE_GRENADE, "High Explosive Grenade" },
	{ ProjectileType::FLASHBANG, "Flashbang" },
};

bool ProjectileTypeSettings::IsEnabled() const
{
	return breachCharge.IsEnabled() || bumpMine.IsEnabled() || decoy.IsEnabled() || molotov.IsEnabled() || sensorGrenade.IsEnabled() || smokeGrenade.IsEnabled() || snowball.IsEnabled() || highExplosiveGrenade.IsEnabled() || flashbang.IsEnabled();
}

ProjectileSettings& ProjectileTypeSettings::GetSettings(ProjectileType type) const
{
	switch (type) {
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
	case ProjectileType::INVALID:
	default:
		__builtin_unreachable();
	}
}

void ProjectileTypeSettings::Draw(ImDrawList* drawList, Projectile& projectile) const
{
	GetSettings(projectile.type).Draw(drawList, projectile, projectileNames[projectile.type]);
}

void ProjectileTypeSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	if (ImGui::BeginTabBar("#Projectile config selection", ImGuiTabBarFlags_Reorderable)) {
		for (const auto& [type, name] : projectileNames) {
			if (ImGui::BeginTabItem(name)) {
				ProjectileSettings& projectileSettings = GetSettings(type);

				if (ImGui::Popup("Copy from", "Copy from")) {
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
	SERIALIZE_STRUCT("Breach charge", breachCharge);
	SERIALIZE_STRUCT("Bump Mine", bumpMine);
	SERIALIZE_STRUCT("Decoy", decoy);
	SERIALIZE_STRUCT("Molotov", molotov);
	SERIALIZE_STRUCT("Sensor Grenade", sensorGrenade);
	SERIALIZE_STRUCT("Smoke Grenade", smokeGrenade);
	SERIALIZE_STRUCT("Snowball", snowball);
	SERIALIZE_STRUCT("High Explosive Grenade", highExplosiveGrenade);
	SERIALIZE_STRUCT("Flashbang", flashbang);
}