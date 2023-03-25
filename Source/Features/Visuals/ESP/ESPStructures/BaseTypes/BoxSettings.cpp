#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

BoxSettings::BoxSettings(const char* id)
	: id(id)
	, enabled(false)
	, color(ImGuiColors::white)
	, rounding(0.0f)
	, thickness(1.0f)
	, outlined(false)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
	, fill(false)
	, fillColor(ImGuiColors::white)
{
}

void BoxSettings::Draw(ImDrawList* drawList, ImVec4 rectangle) const
{
	if (!enabled)
		return;
	if (fill)
		drawList->AddRectFilled(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), fillColor, rounding, ImDrawFlags_None);
	if (outlined)
		drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), outlineColor, rounding, ImDrawFlags_None, thickness + outlineThickness);
	drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), color, rounding, ImDrawFlags_None, thickness);
}

bool BoxSettings::operator==(const BoxSettings& other) const
{
	// clang-format off
	return
		enabled == other.enabled &&
		color == other.color &&
		rounding == other.rounding &&
		thickness == other.thickness &&
		outlined == other.outlined &&
		outlineColor == other.outlineColor &&
		outlineThickness == other.outlineThickness &&
		fill == other.fill &&
		fillColor == other.fillColor;
	// clang-format on
}

void BoxSettings::Copy(BoxSettings& src)
{
	enabled = src.enabled;
	color = src.color;
	rounding = src.rounding;
	thickness = src.thickness;
	outlined = src.outlined;
	outlineColor = src.outlineColor;
	outlineThickness = src.outlineThickness;
	fill = src.fill;
	fillColor = src.fillColor;
}

void BoxSettings::SetupGUI()
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(id);

	if (ImGui::BeginPopup(id)) {
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

BEGIN_SERIALIZED_STRUCT(BoxSettings::Serializer, id)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Color"), color)
SERIALIZED_TYPE(xorstr_("Rounding"), rounding)
SERIALIZED_TYPE(xorstr_("Thickness"), thickness)
SERIALIZED_TYPE(xorstr_("Outlined"), outlined)
SERIALIZED_TYPE(xorstr_("Outline color"), outlineColor)
SERIALIZED_TYPE(xorstr_("Outline thickness"), outlineThickness)
SERIALIZED_TYPE(xorstr_("Fill"), fill)
SERIALIZED_TYPE(xorstr_("Fill color"), fillColor)
END_SERIALIZED_STRUCT
