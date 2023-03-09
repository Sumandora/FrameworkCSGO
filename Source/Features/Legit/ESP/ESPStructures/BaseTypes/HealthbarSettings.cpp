#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

HealthbarSettings::HealthbarSettings(const char* id)
	: id(id)
	, enabled(false)
	, backgroundColor(ImGuiColors::black)
	, rounding(0.0f)
	, spacing(3.0f)
	, width(5.0f)
	, aliveColor(ImGuiColors::green)
	, deadColor(ImGuiColors::red)
	, outlined(false)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
{
}

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

void HealthbarSettings::Copy(HealthbarSettings& src)
{
	enabled = src.enabled;
	backgroundColor = src.backgroundColor;
	rounding = src.rounding;
	spacing = src.spacing;
	width = src.width;
	aliveColor = src.aliveColor;
	deadColor = src.deadColor;
	outlined = src.outlined;
	outlineColor = src.outlineColor;
	outlineThickness = src.outlineThickness;
}

void HealthbarSettings::SetupGUI()
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(id);

	if (ImGui::BeginPopup(id)) {
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

BEGIN_SERIALIZED_STRUCT(HealthbarSettings::Serializer, id)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Background color"), backgroundColor)
SERIALIZED_TYPE(xorstr_("Rounding"), rounding)
SERIALIZED_TYPE(xorstr_("Spacing"), spacing)
SERIALIZED_TYPE(xorstr_("Width"), width)
SERIALIZED_TYPE(xorstr_("Alive color"), aliveColor)
SERIALIZED_TYPE(xorstr_("Dead color"), deadColor)
SERIALIZED_TYPE(xorstr_("Outlined"), outlined)
SERIALIZED_TYPE(xorstr_("Outline color"), outlineColor)
SERIALIZED_TYPE(xorstr_("Outline thickness"), outlineThickness)
END_SERIALIZED_STRUCT
