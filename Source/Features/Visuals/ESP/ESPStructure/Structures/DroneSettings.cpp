#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Interfaces.hpp"

bool DroneSettings::isEnabled() const
{
	return boxName.isEnabled() || target.enabled;
}

void DroneSettings::draw(ImDrawList* drawList, Drone& drone) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = drone.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), "Drone");

	Player* player = EntityCache::playerByHandle(drone.target);
	if (player) {
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(player->index, &info))
			target.draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
	}
}

void DroneSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	target.setupGUI("Target");
	ImGui::PopID();
}

SCOPED_SERIALIZER(DroneSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Target", target);
}