#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../Interfaces.hpp"

void DroneSettings::Draw(ImDrawList* drawList, Drone& drone) const
{
	if (!boxName.IsEnabled() && !target.enabled)
		return;

	const std::optional<ImVec4> rectangle = drone.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), xorstr_("Drone"));

	if (drone.target != INVALID_EHANDLE_INDEX) {
		for(const Player& player : EntityCache::GetPlayers()) {
			if(player.handle == drone.target) {
				PlayerInfo info{};
				Interfaces::engine->GetPlayerInfo(player.index, &info);
				target.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, info.name);
				return;
			}
		}
	}
}

void DroneSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	target.SetupGUI(xorstr_("Target"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(DroneSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Target"), target)
END_SERIALIZED_STRUCT