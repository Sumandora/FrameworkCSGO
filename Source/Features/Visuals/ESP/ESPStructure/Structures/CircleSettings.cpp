#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"

CircleSettings::CircleSettings()
	: enabled(false)
	, circleColor(ImGuiColors::white)
	, radius(3.0f)
	, outlined(true)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
{
}

void CircleSettings::Draw(ImDrawList* drawList, ImVec2 center) const
{
	if (!enabled)
		return;

	if (outlined)
		drawList->AddCircleFilled(center, radius + outlineThickness, outlineColor);

	drawList->AddCircleFilled(center, radius, circleColor);
}

void CircleSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::ClickableColorButton("Circle color", circleColor);
		ImGui::SliderFloat("Radius", &radius, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox("Outlined", &outlined);
		if (outlined) {
			ImGui::ClickableColorButton("Outline color", outlineColor);
			ImGui::SliderFloat("Outline thickness", &outlineThickness, 0.0f, 10.0f, "%.2f");
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

SCOPED_SERIALIZER(CircleSettings)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_VECTOR4D("Circle color", circleColor.Value);
	SERIALIZE("Radius", radius);
	SERIALIZE("Outlined", outlined);
	SERIALIZE_VECTOR4D("Outline color", outlineColor.Value);
	SERIALIZE("Outline thickness", outlineThickness);
}
