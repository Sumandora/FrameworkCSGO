#ifndef GUI_ELEMENTS_KEYBIND
#define GUI_ELEMENTS_KEYBIND

#include "imgui.h"
#include <optional>

bool IsInputDown(int key, bool _default, std::optional<bool> inMenu = false);

namespace ImGui {
	IMGUI_API bool InputSelector(const char* label, int& key, const ImVec2& size = ImVec2(0, 0));
}

#endif
