#include "../ESPStructure.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/Elements/Popup.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"
#include "../../../../../GUI/ImGuiMacros.hpp"
#include <optional>

FlagsSetting::FlagsSetting(std::vector<Flag*> flags)
	: enabled(false)
	, fontScale(1.0f)
	, shadow(true)
	, shadowColor(ImGuiColors::black)
	, flags(flags)
{
}

bool FlagsSetting::IsEnabled() const
{
	if (enabled)
		for (const Flag* flag : flags)
			if (flag->enabled)
				return true;

	return false;
}

void FlagsSetting::Draw(ImDrawList* drawList, float x, float y, const Player& player) const
{
	if (!IsEnabled())
		return;

	if (fontScale <= 0.0f)
		return;

	// Hack
	const float oldFontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale *= fontScale;
	ImGui::PushFont(ImGui::GetFont());

	for (const Flag* flag : flags) {
		if (!flag->enabled)
			continue;

		std::optional<std::string> text = flag->GetText(player);
		if (!text.has_value())
			continue;

		const char* cstring = text->c_str();

		const ImVec2 size = ImGui::CalcTextSize(cstring);
		const ImVec2 position(x, y);

		float alphaScale = flag->GetAlpha(player);

		if (shadow) {

			ImColor shadowColor = this->shadowColor;
			shadowColor.Value.w *= alphaScale;
			drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), shadowColor, cstring);
		}

		ImColor color = flag->color;
		color.Value.w *= alphaScale;
		drawList->AddText(position, color, cstring);

		y += size.y;
	}

	ImGui::GetFont()->Scale = oldFontScale;
	ImGui::PopFont();
}

void FlagsSetting::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	ImGui::Checkbox(id, &enabled);

	ImGui::SameLine();

	if (ImGui::Popup(id)) {
		ImGui::SliderFloat(xorstr_("Font scale"), &fontScale, 0.1f, 2.0f, xorstr_("%.2f"));
		ImGui::Checkbox(xorstr_("Shadow"), &shadow);
		if (shadow)
			ImGui::ClickableColorButton(xorstr_("Shadow color"), shadowColor);

		TABBAR(xorstr_("Flags"), [&]() {
			for (Flag* flag : flags) {
				TABITEM(flag->GetName().c_str(), [&flag]() {
					ImGui::Checkbox(xorstr_("Enabled"), &flag->enabled);
					ImGui::ClickableColorButton(xorstr_("Flag color"), flag->color);
				});
			}
		})

		ImGui::EndPopup();
	}

	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(FlagsSetting::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Font scale"), fontScale)
SERIALIZED_TYPE(xorstr_("Shadow"), shadow)
SERIALIZED_TYPE(xorstr_("Shadow color"), shadowColor)

for (Flag* flag : flags) {
	SERIALIZED_STRUCTURE(flag->GetName().c_str(), (*flag))
}

END_SERIALIZED_STRUCT
