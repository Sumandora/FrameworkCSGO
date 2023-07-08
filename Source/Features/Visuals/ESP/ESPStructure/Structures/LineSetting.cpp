#include "../ESPStructure.hpp"

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
		ImGui::ClickableColorButton("Line color", lineColor);
		ImGui::SliderFloat("Thickness", &thickness, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox("Outlined", &outlined);
		if (outlined) {
			ImGui::ClickableColorButton("Outline color", outlineColor);
			ImGui::SliderFloat("Outline thickness", &outlineThickness, 0.0f, 10.0f, "%.2f");
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

SCOPED_SERIALIZER(LineSetting)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_VECTOR4D("Line color", lineColor.Value);
	SERIALIZE("Thickness", thickness);
	SERIALIZE("Outlined", outlined);
	SERIALIZE_VECTOR4D("Outline color", outlineColor.Value);
	SERIALIZE("Outline thickness", outlineThickness);
}
