#include "../ESPStructure.hpp"

#include "xorstr.hpp"

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
		ImGui::ClickableColorButton(xorstr_("Color"), color);
		ImGui::SliderFloat(xorstr_("Rounding"), &rounding, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Thickness"), &thickness, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::Checkbox(xorstr_("Outlined"), &outlined);
		if (outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &outlineThickness, 0.0f, 10.0f, xorstr_("%.2f"));
		}
		ImGui::Checkbox(xorstr_("Fill"), &fill);
		if (fill)
			ImGui::ClickableColorButton(xorstr_("Fill color"), fillColor);

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

SCOPED_SERIALIZER(BoxSettings)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE_VECTOR4D(xorstr_("Color"), color.Value);
	SERIALIZE(xorstr_("Rounding"), rounding);
	SERIALIZE(xorstr_("Thickness"), thickness);
	SERIALIZE(xorstr_("Outlined"), outlined);
	SERIALIZE_VECTOR4D(xorstr_("Outline color"), outlineColor.Value);
	SERIALIZE(xorstr_("Outline thickness"), outlineThickness);
	SERIALIZE(xorstr_("Fill"), fill);
	SERIALIZE_VECTOR4D(xorstr_("Fill color"), fillColor.Value);
}
