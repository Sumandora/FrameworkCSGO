#include "../ESPStructure.hpp"

bool GenericEntitySettings::isEnabled() const
{
	return boxName.isEnabled();
}

void GenericEntitySettings::draw(ImDrawList* drawList, Entity& entity, const char* text) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = entity.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), text);
}

void GenericEntitySettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	ImGui::PopID();
}

SCOPED_SERIALIZER(GenericEntitySettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
}