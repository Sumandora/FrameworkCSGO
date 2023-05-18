#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../Utils/Projection.hpp"

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

void ProjectileSettings::Draw(ImDrawList* drawList, Projectile& projectile) const
{
	// Render trail even if we don't have a rectangle
	if(trail.enabled) {
		std::vector<ImVec2> points;

		for (const Vector& pos : projectile.trail) {
			ImVec2 point2D;
			if (!Utils::Project(pos, point2D)) {
				if (!points.empty()) {
					trail.Draw(drawList, points);
					points.clear();
				}
				continue;
			}
			points.push_back(point2D);
		}

		trail.Draw(drawList, points);
	}

	if (!boxName.IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = projectile.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	if(projectile.type == ProjectileType::INVALID)
		__asm("int3");

	boxName.Draw(drawList, rectangle.value(), projectileNames[projectile.type]);
}

void ProjectileSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	trail.SetupGUI(xorstr_("Trail"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(ProjectileSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Trail"), trail)
END_SERIALIZED_STRUCT