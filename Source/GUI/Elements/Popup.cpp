#include "Popup.hpp"

#include <cstdio>
#include <cstring>

IMGUI_API bool ImGui::Popup(const char* label, const char* button_label, const ImVec2& size) {
	char new_label[strlen(button_label) + 2 + strlen(label)];
	sprintf(new_label, xorstr_("%s##%s"), button_label, label);

	if (ImGui::Button(new_label, size))
		ImGui::OpenPopup(label);

	return ImGui::BeginPopup(label);
}