#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"

BoxSettings::BoxSettings()
	: enabled(false)
	, color(ImGuiColors::white)
	, rounding(0.0f)
	, thickness(1.0f)
	, outlined(true)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
	, fill(false)
	, fillColor(ImGuiColors::white)
{
}

void BoxSettings::Draw(ImDrawList* drawList, const ImVec4& rectangle) const
{
	if (!enabled)
		return;

	if (fill)
		drawList->AddRectFilled(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), fillColor, rounding, ImDrawFlags_None);

	if (outlined)
		drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), outlineColor, rounding, ImDrawFlags_None, thickness + outlineThickness);

	drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), color, rounding, ImDrawFlags_None, thickness);
}

float BoxSettings::GetLineWidth() const
{
	float width = thickness;
	if (outlined)
		width += outlineThickness;
	return width;
}

void BoxSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::ClickableColorButton("Color", color);
		ImGui::SliderFloat("Rounding", &rounding, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Thickness", &thickness, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox("Outlined", &outlined);
		if (outlined) {
			ImGui::ClickableColorButton("Outline color", outlineColor);
			ImGui::SliderFloat("Outline thickness", &outlineThickness, 0.0f, 10.0f, "%.2f");
		}
		ImGui::Checkbox("Fill", &fill);
		if (fill)
			ImGui::ClickableColorButton("Fill color", fillColor);

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

SCOPED_SERIALIZER(BoxSettings)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_VECTOR4D("Color", color.Value);
	SERIALIZE("Rounding", rounding);
	SERIALIZE("Thickness", thickness);
	SERIALIZE("Outlined", outlined);
	SERIALIZE_VECTOR4D("Outline color", outlineColor.Value);
	SERIALIZE("Outline thickness", outlineThickness);
	SERIALIZE("Fill", fill);
	SERIALIZE_VECTOR4D("Fill color", fillColor.Value);
}
