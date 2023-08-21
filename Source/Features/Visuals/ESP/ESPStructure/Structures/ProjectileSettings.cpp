#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Utils/Projection.hpp"

#include "../../../../../Interfaces.hpp"

#include <cstdio>

// TODO Allow separation based on the throwers team (local, team, enemy)

bool ProjectileSettings::isEnabled() const
{
	return boxName.isEnabled() || ownerName.enabled || trail.enabled;
}

void ProjectileSettings::draw(ImDrawList* drawList, Projectile& projectile, const char* name) const
{
	if (projectile.type == ProjectileType::INVALID) {
		__asm("int3");
		return;
	}

	if (!isEnabled())
		return;

	// Render trail even if we don't have a rectangle
	if (trail.enabled) {
		std::vector<ImVec2> points;

		for (const Vector& pos : projectile.trail) {
			ImVec2 point2D;
			if (!Utils::project(pos, point2D)) {
				if (!points.empty()) {
					trail.draw(drawList, points);
					points.clear();
				}
				continue;
			}
			points.push_back(point2D);
		}

		trail.draw(drawList, points);
	}

	const std::optional<ImVec4> rectangle = projectile.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), name);

	Player* player = EntityCache::playerByHandle(projectile.thrower);
	if (player) {
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(player->index, &info))
			ownerName.draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
	}
}

void ProjectileSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	ownerName.setupGUI("Owner name");
	trail.setupGUI("Trail");
	ImGui::PopID();
}

SCOPED_SERIALIZER(ProjectileSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Owner name", ownerName);
	SERIALIZE_STRUCT("Trail", trail);
}