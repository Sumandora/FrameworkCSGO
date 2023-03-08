#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void TextSetting::Draw(ImDrawList* drawList, ImVec4 rectangle, const char* text, float height)
{
	if (!enabled)
		return;

	// Hack
	float fontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale = fontScale;
	ImGui::PushFont(ImGui::GetFont());

	ImVec2 size = ImGui::CalcTextSize(text);

	float above = rectangle.y - size.y;
	float below = rectangle.w;
	ImVec2 position(rectangle.x + (rectangle.z - rectangle.x) * 0.5f - size.x / 2.0f, above + (below - above) * height);

	if (shadow)
		drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), shadowColor, text);

	drawList->AddText(position, fontColor, text);

	ImGui::PopFont();
	ImGui::GetFont()->Scale = fontScale;
}

void TextSetting::SetupGUI(const char* tag)
{
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &enabled);

	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if (ImGui::BeginPopup(tag)) {
		ImGui::SliderFloat(xorstr_("Font scale"), &fontScale, 0.1f, 2.0f, "%.2f");
		ImGui::ClickableColorButton(xorstr_("Font color"), fontColor);
		ImGui::Checkbox(xorstr_("Shadow"), &shadow);
		if (shadow)
			ImGui::ClickableColorButton(xorstr_("Shadow color"), shadowColor);

		ImGui::EndPopup();
	}

	ImGui::PopID();
}