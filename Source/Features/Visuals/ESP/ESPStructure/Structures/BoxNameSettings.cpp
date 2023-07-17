#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

bool BoxNameSettings::IsEnabled() const
{
	return box.enabled || nametag.enabled;
}

void BoxNameSettings::Draw(ImDrawList* drawList, const ImVec4& rectangle, const char* text) const
{
	if (!IsEnabled())
		return;

	box.Draw(drawList, rectangle);
	nametag.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.y - nametag.GetLineHeight(), true, text);
}

void BoxNameSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	box.SetupGUI("Box");
	nametag.SetupGUI("Name");
	ImGui::PopID();
}

SCOPED_SERIALIZER(BoxNameSettings)
{
	SERIALIZE_STRUCT("Box", box);
	SERIALIZE_STRUCT("Name", nametag);
}
