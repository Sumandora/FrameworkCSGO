#include "../ESPStructure.hpp"

#include "imgui.h"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"
#include <memory>
#include <optional>
#include <utility>

FlagsSettings::FlagsSettings(std::vector<std::shared_ptr<Flag>>&& flags)
	: enabled(false)
	, fontScale(1.0f)
	, verticalSpacing(1.0f)
	, shadow(true)
	, shadowColor(ImGuiColors::black)
	, flags(std::move(flags))
{
}

bool FlagsSettings::isEnabled() const
{
	if (enabled)
		for (const std::shared_ptr<Flag>& flag : flags)
			if (flag->enabled)
				return true;

	return false;
}

void FlagsSettings::draw(ImDrawList* drawList, float x, float y, const Player& player) const
{
	if (!isEnabled())
		return;

	if (fontScale <= 0.0f)
		return;

	// Hack
	const float oldFontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale *= fontScale;
	ImGui::PushFont(ImGui::GetFont());

	for (const std::shared_ptr<Flag>& flag : flags) {
		if (!flag->enabled)
			continue;

		std::optional<std::string> text = flag->getText(player);
		if (!text.has_value())
			continue;

		const char* cstring = text->c_str();

		const ImVec2 size = ImGui::CalcTextSize(cstring);
		const ImVec2 position(x, y);

		float alphaScale = flag->getAlpha(player);

		if (shadow) {

			ImColor shadowColor = this->shadowColor;
			shadowColor.Value.w *= alphaScale;
			drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), shadowColor, cstring);
		}

		ImColor color = flag->color;
		color.Value.w *= alphaScale;
		drawList->AddText(position, color, cstring);

		y += size.y * verticalSpacing;
	}

	ImGui::GetFont()->Scale = oldFontScale;
	ImGui::PopFont();
}

void FlagsSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::SliderFloat("Font scale", &fontScale, 0.1f, 2.0f, "%.2f");
		ImGui::SliderFloat("Vertical spacing", &verticalSpacing, 0.1f, 1.0f, "%.2f");
		ImGui::Checkbox("Shadow", &shadow);
		if (shadow)
			ImGui::ClickableColorButton("Shadow color", shadowColor);

		if (ImGui::BeginTabBar("Flags", ImGuiTabBarFlags_Reorderable)) {
			for (const std::shared_ptr<Flag>& flag : flags) {
				if (ImGui::BeginTabItem(flag->getName().c_str())) {
					ImGui::Checkbox("Enabled", &flag->enabled);
					ImGui::ClickableColorButton("Flag color", flag->color);
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

SCOPED_SERIALIZER(FlagsSettings)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Font scale", fontScale);
	SERIALIZE("Vertical spacing", verticalSpacing);
	SERIALIZE("Shadow", shadow);
	SERIALIZE_VECTOR4D("Shadow color", shadowColor.Value);

	for (const std::shared_ptr<Flag>& flag : flags) {
		SERIALIZE_STRUCT(flag->getName().c_str(), (*flag));
	}
}
