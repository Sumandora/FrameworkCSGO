// This file is only targeted if MENUPREVIEW (in CMakeLists.txt) has been enabled

#include "imgui.h"
#include "GUI/GUI.hpp"

void ImGui::ShowDemoWindow(bool* p_open) {
	if(!*p_open)
		return;

	SDL_Window* window = SDL_GL_GetCurrentWindow();

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	Gui::BuildMenu(width, height);
}