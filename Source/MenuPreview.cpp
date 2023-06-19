// This file is only targeted if MENUPREVIEW (in CMakeLists.txt) has been enabled

#include "GUI/GUI.hpp"
#include "imgui.h"

void ImGui::ShowDemoWindow(bool* p_open)
{
	if (!*p_open)
		return;

	SDL_Window* window = SDL_GL_GetCurrentWindow();

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	Gui::Build();
}