#include "GUI.hpp"

#include "Hooks/SDL/SDLHook.hpp"
#include "Hooks/DX9/DX9Hook.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_dx9.h"

#include <SDL.h>
#include <algorithm>

bool visible = false;
SDL_Window* windowPtr;

void Gui::Create() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
}

void Gui::SwapWindow(SDL_Window* window) {
	windowPtr = window;

	if(!Hooks::DX9::device)
		return;
	
	// This hack is also used by Osiris, because of the 'static' keyword it is only executed once
	static const auto _1 = ImGui_ImplSDL2_InitForD3D(window);
	static const auto _2 = ImGui_ImplDX9_Init((IDirect3DDevice9*) Hooks::DX9::device);

	ImGuiIO& io = ImGui::GetIO();

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	io.DisplaySize = ImVec2((float)w, (float)h);

	io.MousePos.x = std::clamp(io.MousePos.x, 0.0f, (float)w);
	io.MousePos.y = std::clamp(io.MousePos.y, 0.0f, (float)h);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	
	if(visible) {
		ImGui::ShowDemoWindow();
	}
	
	if (ImGui::IsKeyPressed(SDL_SCANCODE_INSERT, false)) {
		visible = !visible;
	}

	io.MouseDrawCursor = visible;
	io.WantCaptureMouse = visible;
	io.WantCaptureKeyboard = visible;

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Gui::PollEvent(SDL_Event* event, int result) {
	if(
		event->type != SDL_MOUSEWHEEL &&
		event->type != SDL_MOUSEBUTTONDOWN &&
		event->type != SDL_MOUSEBUTTONUP &&
		event->type != SDL_TEXTINPUT &&
		event->type != SDL_KEYDOWN &&
		event->type != SDL_KEYUP &&
		event->type != SDL_MOUSEMOTION
	) return;

	ImGuiIO& io = ImGui::GetIO();
	if(visible && event->type == SDL_MOUSEBUTTONUP) {
		reinterpret_cast<void(*)(SDL_Window*,int,int)>(Hooks::SDL::warpMouseInWindow_proxy)(windowPtr, io.MousePos.x, io.MousePos.y);
	}
	
	if(event->type != SDL_MOUSEMOTION)
		ImGui_ImplSDL2_ProcessEvent(event);
	else if(visible) {
		io.MousePos.x += event->motion.xrel;
		io.MousePos.y += event->motion.yrel;
	}
	
	if(visible && result)
		event->type = 0;
}

bool Gui::WarpMouseInWindow() {
	// lol no	
	return visible;
}
