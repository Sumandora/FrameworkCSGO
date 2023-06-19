#ifndef GUI_IMGUIMACROS
#define GUI_IMGUIMACROS

#include "imgui.h"

#define TABBAR(name, constructor)                                 \
	if (ImGui::BeginTabBar(name, ImGuiTabBarFlags_Reorderable)) { \
		ImGui::PushID(name);                                      \
		constructor();                                            \
		ImGui::PopID();                                           \
		ImGui::EndTabBar();                                       \
	}

#define TABITEM(name, item)          \
	if (ImGui::BeginTabItem(name)) { \
		ImGui::PushID(name);         \
		item();                      \
		ImGui::PopID();              \
		ImGui::EndTabItem();         \
	}

#endif