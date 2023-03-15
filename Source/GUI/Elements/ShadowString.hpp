#ifndef GUI_ELEMENTS_SHADOWSTRING
#define GUI_ELEMENTS_SHADOWSTRING

#include "imgui.h"

namespace ShadowString {
	IMGUI_API void AddText(ImDrawList* drawList, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = nullptr);
	IMGUI_API void AddText(ImDrawList* drawList, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = nullptr, float wrap_width = 0.0F, const ImVec4* cpu_fine_clip_rect = nullptr);
}

#endif
