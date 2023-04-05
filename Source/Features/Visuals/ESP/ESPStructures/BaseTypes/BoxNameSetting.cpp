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
SERIALIZED_STRUCTURE(xorstr_("Box"), box)
SERIALIZED_STRUCTURE(xorstr_("Name"), nametag)
END_SERIALIZED_STRUCT
