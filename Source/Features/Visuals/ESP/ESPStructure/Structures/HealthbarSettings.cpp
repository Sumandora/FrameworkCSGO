#include "../ESPStructure.hpp"

#include <algorithm>

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"

HealthbarSettings::HealthbarSettings()
	: enabled(false)
	, backgroundColor(ImGuiColors::black)
	, rounding(0.0f)
	, spacing(3.0f)
	, width(5.0f)
	, aliveColor(ImGuiColors::green)
	, deadColor(ImGuiColors::red)
	, gradient(false)
	, outlined(true)
	, outlineColor(ImGuiColors::black)
	, outlineThickness(1.0f)
	, onlyWhenDamaged(true)
{
}

void HealthbarSettings::Draw(ImDrawList* drawList, const ImVec4& rectangle, int health, int maxHealth) const
{
	if (!enabled)
		return;

	const float healthPercentage = std::clamp((float)health / (float)maxHealth, 0.0f, 1.0f);

	const ImVec4 healthbar(rectangle.x - spacing - width, rectangle.y, rectangle.x - spacing, rectangle.w);
	const ImVec4 outside(healthbar.x - outlineThickness, healthbar.y - outlineThickness, healthbar.z + outlineThickness, healthbar.w + outlineThickness);

	ImVec4 background;

	if (outlined) // If we are outlining it, we don't want a gap between the outline and the healthbar
		background = outside;
	else
		background = healthbar;

	float effectiveRounding;
	if (gradient)
		effectiveRounding = 0.0f; // ImGui Limitation
	else
		effectiveRounding = rounding;

	drawList->AddRectFilled(ImVec2(background.x, background.y), ImVec2(background.z, background.w), backgroundColor, effectiveRounding);
	if (outlined) {
		drawList->AddRect(ImVec2(outside.x, outside.y), ImVec2(outside.z, outside.w), outlineColor, effectiveRounding, ImDrawFlags_None, outlineThickness);
	}

	const float healthbarHeight = healthbar.y + (healthbar.w - healthbar.y) * (1.0f - healthPercentage);

	float aliveHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		aliveColor.Value.x, aliveColor.Value.y, aliveColor.Value.z,
		aliveHsv[0], aliveHsv[1], aliveHsv[2]);

	float deadHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		deadColor.Value.x, deadColor.Value.y, deadColor.Value.z,
		deadHsv[0], deadHsv[1], deadHsv[2]);

	if (gradient) {
		const float transition = std::min(0.5f, healthPercentage); // At some point the second rect will become smaller than 50% means we have to adjust the color in order to not break the illusion of it being one long bar
		const float middleHsv[] = {
			deadHsv[0] + (aliveHsv[0] - deadHsv[0]) * transition,
			deadHsv[1] + (aliveHsv[1] - deadHsv[1]) * transition,
			deadHsv[2] + (aliveHsv[2] - deadHsv[2]) * transition
		};
		const float alpha = deadColor.Value.w + (aliveColor.Value.w - deadColor.Value.w) * transition;

		float middleRgb[3];
		ImGui::ColorConvertHSVtoRGB(
			middleHsv[0], middleHsv[1], middleHsv[2],
			middleRgb[0], middleRgb[1], middleRgb[2]);

		const ImColor middleColor(middleRgb[0], middleRgb[1], middleRgb[2], alpha);

		const float halfHeight = healthbar.w + (healthbar.y - healthbar.w) * 0.5f;

		if (healthPercentage > 0.5) {
			drawList->AddRectFilledMultiColor(ImVec2(healthbar.x, healthbarHeight), ImVec2(healthbar.z, halfHeight), aliveColor, aliveColor, middleColor, middleColor);
		}
		drawList->AddRectFilledMultiColor(ImVec2(healthbar.x, std::max /* Remember that these are screen coords, not percentages (min -> max) */ (halfHeight, healthbarHeight)), ImVec2(healthbar.z, healthbar.w), middleColor, middleColor, deadColor, deadColor);
	} else {
		const float desiredHsv[] = {
			deadHsv[0] + (aliveHsv[0] - deadHsv[0]) * healthPercentage,
			deadHsv[1] + (aliveHsv[1] - deadHsv[1]) * healthPercentage,
			deadHsv[2] + (aliveHsv[2] - deadHsv[2]) * healthPercentage
		};
		const float alpha = deadColor.Value.w + (aliveColor.Value.w - deadColor.Value.w) * healthPercentage;

		float desiredRgb[3];
		ImGui::ColorConvertHSVtoRGB(
			desiredHsv[0], desiredHsv[1], desiredHsv[2],
			desiredRgb[0], desiredRgb[1], desiredRgb[2]);

		const ImColor color(desiredRgb[0], desiredRgb[1], desiredRgb[2], alpha);

		drawList->AddRectFilled(ImVec2(healthbar.x, healthbarHeight), ImVec2(healthbar.z, healthbar.w), color, effectiveRounding);
	}

	if (healthNumber.enabled) {
		if (!onlyWhenDamaged || healthPercentage < 1.0f)
			healthNumber.Draw(drawList, healthbar.x + (healthbar.z - healthbar.x) * 0.5f, healthbarHeight - healthNumber.GetLineHeight(), true, std::to_string(health).c_str());
	}
}

void HealthbarSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::ClickableColorButton(xorstr_("Background color"), backgroundColor);
		if (!gradient)
			ImGui::SliderFloat(xorstr_("Rounding"), &rounding, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Spacing"), &spacing, 0.0f, 10.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Width"), &width, 0.0f, 10.0f, xorstr_("%.2f"));

		ImGui::ClickableColorButton(xorstr_("Alive color"), aliveColor);
		ImGui::ClickableColorButton(xorstr_("Dead color"), deadColor);
		ImGui::Checkbox(xorstr_("Gradient"), &gradient);

		ImGui::Checkbox(xorstr_("Outlined"), &outlined);
		if (outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &outlineThickness, 0.0f, 10.0f, xorstr_("%.2f"));
		}
		healthNumber.SetupGUI(xorstr_("Health number"));
		if (healthNumber.enabled)
			ImGui::Checkbox(xorstr_("Only when damaged"), &onlyWhenDamaged);

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(HealthbarSettings::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Background color"), backgroundColor)
SERIALIZED_TYPE(xorstr_("Rounding"), rounding)
SERIALIZED_TYPE(xorstr_("Spacing"), spacing)
SERIALIZED_TYPE(xorstr_("Width"), width)
SERIALIZED_TYPE(xorstr_("Alive color"), aliveColor)
SERIALIZED_TYPE(xorstr_("Dead color"), deadColor)
SERIALIZED_TYPE(xorstr_("Gradient"), gradient)
SERIALIZED_TYPE(xorstr_("Outlined"), outlined)
SERIALIZED_TYPE(xorstr_("Outline color"), outlineColor)
SERIALIZED_TYPE(xorstr_("Outline thickness"), outlineThickness)

SERIALIZED_STRUCTURE(xorstr_("Health number"), healthNumber)

SERIALIZED_TYPE(xorstr_("Only when damaged"), onlyWhenDamaged)
END_SERIALIZED_STRUCT
