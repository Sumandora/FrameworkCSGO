#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void BoxSettings::Draw(ImDrawList* drawList, ImVec4 rectangle)
{
	if (!enabled)
		return;
	if (fill)
		drawList->AddRectFilled(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), fillColor, rounding, ImDrawFlags_None);
	if (outlined)
		drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), outlineColor, rounding, ImDrawFlags_None, thickness + outlineThickness);
	drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), color, rounding, ImDrawFlags_None, thickness);
}

void BoxSettings::SetupGUI(const char* tag)
{
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &enabled);

	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if (ImGui::BeginPopup(tag)) {
		ImGui::ClickableColorButton(xorstr_("Color"), color);
		ImGui::SliderFloat(xorstr_("Rounding"), &rounding, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Thickness"), &thickness, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox(xorstr_("Outlined"), &outlined);
		if (outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &outlineThickness, 0.0f, 10.0f, "%.2f");
		}
		ImGui::Checkbox(xorstr_("Fill"), &fill);
		if (fill)
			ImGui::ClickableColorButton(xorstr_("Fill color"), fillColor);

		ImGui::EndPopup();
	}

	ImGui::PopID();
}