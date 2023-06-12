#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Utils/Projection.hpp"

#include "../../../../../Interfaces.hpp"
#include <cstdio>

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
		Interfaces::engine->GetPlayerInfo(player->index, &info);
		ownerName.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
	}
}

void ProjectileSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	ownerName.SetupGUI(xorstr_("Owner name"));
	trail.SetupGUI(xorstr_("Trail"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(ProjectileSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Owner name"), ownerName)
SERIALIZED_STRUCTURE(xorstr_("Trail"), trail)
END_SERIALIZED_STRUCT