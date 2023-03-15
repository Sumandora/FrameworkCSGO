#include "ShadowString.hpp"

#include "../ImGuiColors.hpp"

IMGUI_API void ShadowString::AddText(ImDrawList* drawList, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
	drawList->AddText(ImVec2(pos.x + 1.0F, pos.y + 1.0F), ImGuiColors::black, text_begin, text_end);
	drawList->AddText(pos, col, text_begin, text_end);
}

IMGUI_API void ShadowString::AddText(ImDrawList* drawList, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
	drawList->AddText(font, font_size, ImVec2(pos.x + 1.0F, pos.y + 1.0F), ImGuiColors::black, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
	drawList->AddText(font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
}