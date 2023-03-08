#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void BoxNameSetting::Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text) {
	box.Draw(drawList, rectangle);
	nametag.Draw(drawList, rectangle, text, 0.0f);
}

void BoxNameSetting::SetupGUI(const char* tag) {
	ImGui::PushID(tag);
	box.SetupGUI(xorstr_("Box"));
	nametag.SetupGUI(xorstr_("Name"));
	ImGui::PopID();
}