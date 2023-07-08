#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Interfaces.hpp"

bool DroneSettings::IsEnabled() const
{
	return boxName.IsEnabled() || target.enabled;
}

void DroneSettings::Draw(ImDrawList* drawList, Drone& drone) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = drone.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), "Drone");

	Player* player = EntityCache::PlayerByHandle(drone.target);
	if (player) {
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(player->index, &info))
			target.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
	}
}

void DroneSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	target.SetupGUI("Target");
	ImGui::PopID();
}

SCOPED_SERIALIZER(DroneSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Target", target);
}