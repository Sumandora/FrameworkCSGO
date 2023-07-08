#include "ClickableColorButton.hpp"

static ImColor clipboard;

IMGUI_API void ImGui::ClickableColorButton(const char* desc_id, ImColor& col, ImGuiColorEditFlags flags, const ImVec2& size)
{
	ImGui::PushID(desc_id);

	const bool clicked = ImGui::ColorButton(desc_id, col.Value, flags, size);
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::Selectable("Copy")) {
			clipboard = col;
		}
		if (ImGui::Selectable("Paste")) {
			col = clipboard;
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	ImGui::Text("%s", desc_id);

	if (clicked)
		ImGui::OpenPopup("##Picker");

	if (ImGui::BeginPopup("##Picker")) {
		float color[] = { col.Value.x, col.Value.y, col.Value.z, col.Value.w };
		ImGui::ColorPicker4(desc_id, color, flags);
		col.Value = ImVec4(color[0], color[1], color[2], color[3]);
		ImGui::EndPopup();
	}

	ImGui::PopID();
}