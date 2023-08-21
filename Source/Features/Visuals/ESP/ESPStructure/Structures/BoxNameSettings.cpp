#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool BoxNameSettings::isEnabled() const
{
	return box.enabled || nametag.enabled;
}

void BoxNameSettings::draw(ImDrawList* drawList, const ImVec4& rectangle, const char* text) const
{
	if (!isEnabled())
		return;

	box.draw(drawList, rectangle);
	nametag.draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.y - nametag.getLineHeight(), true, text);
}

void BoxNameSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	box.setupGUI("Box");
	nametag.setupGUI("Name");
	ImGui::PopID();
}

SCOPED_SERIALIZER(BoxNameSettings)
{
	SERIALIZE_STRUCT("Box", box);
	SERIALIZE_STRUCT("Name", nametag);
}
