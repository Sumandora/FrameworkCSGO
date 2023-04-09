#ifndef GUI_ELEMENTS_POPUP
#define GUI_ELEMENTS_POPUP

#include "imgui.h"
#include "xorstr.hpp"

namespace ImGui {
	IMGUI_API bool Popup(const char* label, const char* button_label = xorstr_("..."), const ImVec2& size = ImVec2(0, 0));
}

#endif
