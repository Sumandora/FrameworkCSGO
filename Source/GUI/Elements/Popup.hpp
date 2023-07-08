#ifndef GUI_ELEMENTS_POPUP
#define GUI_ELEMENTS_POPUP

#include "imgui.h"

namespace ImGui {
	IMGUI_API bool Popup(const char* label, const char* button_label = "...", const ImVec2& size = ImVec2(0, 0));
}

#endif
