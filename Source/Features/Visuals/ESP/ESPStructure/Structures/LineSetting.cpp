#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"

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
		drawList->AddPolyline(points.data(), (int)points.size(), outlineColor, ImDrawFlags_None, thickness + outlineThickness);

	drawList->AddPolyline(points.data(), (int)points.size(), lineColor, ImDrawFlags_None, thickness);
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

SCOPED_SERIALIZER(LineSetting)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE_VECTOR4D(xorstr_("Line color"), lineColor.Value);
	SERIALIZE(xorstr_("Thickness"), thickness);
	SERIALIZE(xorstr_("Outlined"), outlined);
	SERIALIZE_VECTOR4D(xorstr_("Outline color"), outlineColor.Value);
	SERIALIZE(xorstr_("Outline thickness"), outlineThickness);
}
