#include "Features/Visuals/ESP/ESPStructure.hpp"

#include "xorstr.hpp"

#include "GUI/Elements/ClickableColorButton.hpp"
#include "GUI/Elements/Popup.hpp"
#include "GUI/ImGuiColors.hpp"

LineSetting::LineSetting()
	: enabled(false)
	, lineColor(ImGuiColors::white)
	, thickness(1.0f)
	, outlined(true)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
{
}

void LineSetting::Draw(ImDrawList* drawList, std::vector<ImVec2> points) const
{
	if (!enabled)
		return;

	if (outlined)
		drawList->AddPolyline(points.data(), points.size(), outlineColor, ImDrawFlags_None, thickness + outlineThickness);

	drawList->AddPolyline(points.data(), points.size(), lineColor, ImDrawFlags_None, thickness);
}

void LineSetting::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::ClickableColorButton(xorstr_("Line color"), lineColor);
		ImGui::SliderFloat(xorstr_("Thickness"), &thickness, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::Checkbox(xorstr_("Outlined"), &outlined);
		if (outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &outlineThickness, 0.0f, 10.0f, xorstr_("%.2f"));
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(LineSetting::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Line color"), lineColor)
SERIALIZED_TYPE(xorstr_("Thickness"), thickness)
SERIALIZED_TYPE(xorstr_("Outlined"), outlined)
SERIALIZED_TYPE(xorstr_("Outline color"), outlineColor)
SERIALIZED_TYPE(xorstr_("Outline thickness"), outlineThickness)
END_SERIALIZED_STRUCT
