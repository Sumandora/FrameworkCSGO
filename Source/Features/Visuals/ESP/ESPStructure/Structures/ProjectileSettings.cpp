#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Utils/Projection.hpp"

#include "../../../../../Interfaces.hpp"

#include <cstdio>

// TODO Allow separation based on the throwers team (local, team, enemy)

bool ProjectileSettings::IsEnabled() const
{
	return boxName.IsEnabled() || ownerName.enabled || trail.enabled;
}

void ProjectileSettings::Draw(ImDrawList* drawList, Projectile& projectile, const char* name) const
{
	if (projectile.type == ProjectileType::INVALID) {
		__asm("int3");
		return;
	}

	if (!IsEnabled())
		return;

	// Render trail even if we don't have a rectangle
	if (trail.enabled) {
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

	const std::optional<ImVec4> rectangle = projectile.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), name);

	Player* player = EntityCache::PlayerByHandle(projectile.thrower);
	if (player) {
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(player->index, &info))
			ownerName.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
	}
}

void ProjectileSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	ownerName.SetupGUI("Owner name");
	trail.SetupGUI("Trail");
	ImGui::PopID();
}

SCOPED_SERIALIZER(ProjectileSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Owner name", ownerName);
	SERIALIZE_STRUCT("Trail", trail);
}