#ifndef GUI_ELEMENTS_CLICKABLECOLORBUTTON
#define GUI_ELEMENTS_CLICKABLECOLORBUTTON

#include "imgui.h"

namespace ImGui {
    IMGUI_API void ClickableColorButton(const char* desc_id, ImColor& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
}

#endif
