#include "../ESPStructure.hpp"

bool GenericEntitySettings::IsEnabled() const
{
	return boxName.IsEnabled();
}

void GenericEntitySettings::Draw(ImDrawList* drawList, Entity& entity, const char* text) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = entity.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), text);
}

void GenericEntitySettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	ImGui::PopID();
}

SCOPED_SERIALIZER(GenericEntitySettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
}