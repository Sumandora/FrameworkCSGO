#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void HealthbarSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, float health)
{
	if (!enabled)
		return;

	ImVec4 healthbar(rectangle.x - spacing - width, rectangle.y, rectangle.x - spacing, rectangle.w);
	ImVec4 outside(healthbar.x - outlineThickness, healthbar.y - outlineThickness, healthbar.z + outlineThickness, healthbar.w + outlineThickness);

	ImVec4 background;

	if (outlined) // If we are outlining it, we don't want a gap between the outline and the healthbar
		background = outside;
	else
		background = healthbar;

	drawList->AddRectFilled(ImVec2(background.x, background.y), ImVec2(background.z, background.w), backgroundColor, rounding);
	if (outlined) {
		drawList->AddRect(ImVec2(outside.x, outside.y), ImVec2(outside.z, outside.w), outlineColor, rounding, ImDrawFlags_None, outlineThickness);
	}

	float aliveHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		aliveColor.Value.x, aliveColor.Value.y, aliveColor.Value.z,
		aliveHsv[0], aliveHsv[1], aliveHsv[2]);

	float deadHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		deadColor.Value.x, deadColor.Value.y, deadColor.Value.z,
		deadHsv[0], deadHsv[1], deadHsv[2]);

	float desiredHsv[] = {
		deadHsv[0] + (aliveHsv[0] - deadHsv[0]) * health,
		deadHsv[1] + (aliveHsv[1] - deadHsv[1]) * health,
		deadHsv[2] + (aliveHsv[2] - deadHsv[2]) * health
	};
	float alpha = deadColor.Value.w + (aliveColor.Value.w - deadColor.Value.w) * health;

	float desiredRgb[3];
	ImGui::ColorConvertHSVtoRGB(
		desiredHsv[0], desiredHsv[1], desiredHsv[2],
		desiredRgb[0], desiredRgb[1], desiredRgb[2]);

	ImColor color(desiredRgb[0], desiredRgb[1], desiredRgb[2], alpha);
	drawList->AddRectFilled(ImVec2(healthbar.x, healthbar.y + (healthbar.w - healthbar.y) * (1.0 - health)), ImVec2(healthbar.z, healthbar.w), color, rounding);
}

void HealthbarSettings::SetupGUI(const char* tag)
{
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &enabled);

	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if (ImGui::BeginPopup(tag)) {
		ImGui::ClickableColorButton(xorstr_("Background color"), backgroundColor);
		ImGui::SliderFloat(xorstr_("Rounding"), &rounding, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Spacing"), &spacing, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Width"), &width, 0.0f, 10.0f, "%.2f");

		ImGui::ClickableColorButton(xorstr_("Alive color"), aliveColor);
		ImGui::ClickableColorButton(xorstr_("Dead color"), deadColor);

		ImGui::Checkbox(xorstr_("Outlined"), &outlined);
		if (outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &outlineThickness, 0.0f, 10.0f, "%.2f");
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}