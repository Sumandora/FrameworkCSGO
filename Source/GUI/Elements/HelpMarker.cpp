#include "HelpMarker.hpp"

#include "xorstr.hpp"

// Taken from the imgui demo

IMGUI_API void ImGui::HelpMarker(const char* desc)
{
	ImGui::SameLine();
	ImGui::TextDisabled(xorstr_("(?)"));
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}