#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

BoxNameSetting::BoxNameSetting(const char* id)
	: id(id)
	, box(BoxSettings(strdup(xorstr_("Box"))))
	, nametag(TextSetting(strdup(xorstr_("Name"))))
{
}

void BoxNameSetting::Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) const
{
	box.Draw(drawList, rectangle);
	nametag.Draw(drawList, rectangle, text, 0.0F);
}

void BoxNameSetting::Copy(BoxNameSetting& src)
{
	box.Copy(src.box);
	nametag.Copy(src.nametag);
}

void BoxNameSetting::SetupGUI()
{
	ImGui::PushID(id);
	box.SetupGUI();
	nametag.SetupGUI();
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(BoxNameSetting::Serializer, id)
SERIALIZED_STRUCTURE(box)
SERIALIZED_STRUCTURE(nametag)
END_SERIALIZED_STRUCT
