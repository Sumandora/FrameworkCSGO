#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

BoxNameSetting::BoxNameSetting()
	: box(BoxSettings())
	, nametag(TextSetting())
{
}

void BoxNameSetting::Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) const
{
	box.Draw(drawList, rectangle);
	nametag.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.y - nametag.GetLineHeight(), true, text);
}

void BoxNameSetting::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	box.SetupGUI(xorstr_("Box"));
	nametag.SetupGUI(xorstr_("Name"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(BoxNameSetting::Serializer)
SERIALIZED_STRUCTURE(box, xorstr_("Box"))
SERIALIZED_STRUCTURE(nametag, xorstr_("Name"))
END_SERIALIZED_STRUCT
