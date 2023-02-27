#include "GUI.hpp"

#include <algorithm>

#include "imgui.h"
#include "xorstr.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

#include "../Hooks/SDL/SDLHook.hpp"

#include "../Features/Legit/Aimbot.hpp"
#include "../Features/Legit/Bhop.hpp"
#include "../Features/Legit/ESP.hpp"
#include "../Features/Legit/SpectatorList.hpp"
#include "../Features/Legit/Triggerbot.hpp"

#include "../Features/Semirage/Aimbot.hpp"
#include "../Features/Semirage/RecoilAssistance.hpp"

#include "../Netvars.hpp"

bool visible = false;

void Gui::Create() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Might not work on certain distros/configurations
	io.Fonts->AddFontFromFileTTF(xorstr_("/usr/share/fonts/noto/NotoSans-Regular.ttf"), 24.0f);

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
}

void Gui::Destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Gui::SwapWindow(SDL_Window* window) {
	// This hack is also used by Osiris, because of the 'static' keyword it is only executed once
	[[maybe_unused]] static const auto _1 = ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
	[[maybe_unused]] static const auto _2 = ImGui_ImplOpenGL3_Init();

	ImGuiIO&						   io = ImGui::GetIO();

	int								   w, h;
	SDL_GetWindowSize(window, &w, &h);
	io.DisplaySize = ImVec2((float)w, (float)h);

	io.MousePos.x  = std::clamp(io.MousePos.x, 0.0f, (float)w);
	io.MousePos.y  = std::clamp(io.MousePos.y, 0.0f, (float)h);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	if (visible) {
		ImGui::Begin(xorstr_("Framework"));

		ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Once);

		if (ImGui::BeginTabBar(xorstr_("#Settings"), ImGuiTabBarFlags_Reorderable)) {
			if (ImGui::BeginTabItem(xorstr_("Legit"))) {
				if (ImGui::BeginTabBar(xorstr_("#Legit Settings"), ImGuiTabBarFlags_Reorderable)) {
					if (ImGui::BeginTabItem(xorstr_("Aimbot"))) {
						Features::Legit::Aimbot::SetupGUI();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(xorstr_("ESP"))) {
						Features::Legit::Esp::SetupGUI();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(xorstr_("Bhop"))) {
						Features::Legit::Bhop::SetupGUI();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(xorstr_("Triggerbot"))) {
						Features::Legit::Triggerbot::SetupGUI();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(xorstr_("Spectator List"))) {
						Features::Legit::SpectatorList::SetupGUI();
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(xorstr_("Semirage"))) {
				if (ImGui::BeginTabBar(xorstr_("#Semirage Settings"), ImGuiTabBarFlags_Reorderable)) {
					if (ImGui::BeginTabItem(xorstr_("Aimbot"))) {
						Features::Semirage::Aimbot::SetupGUI();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(xorstr_("Recoil assistance"))) {
						Features::Semirage::RecoilAssistance::SetupGUI();
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(xorstr_("Debug"))) {
				if (ImGui::BeginTabBar(xorstr_("#Debug Settings"), ImGuiTabBarFlags_Reorderable)) {
					if (ImGui::BeginTabItem(xorstr_("Netvars"))) {
						Netvars::BuildGUI();
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	Features::Legit::Esp::ImGuiRender(ImGui::GetBackgroundDrawList());
	Features::Legit::SpectatorList::ImGuiRender(ImGui::GetBackgroundDrawList());

	io.MouseDrawCursor	   = visible;
	io.WantCaptureMouse	   = visible;
	io.WantCaptureKeyboard = visible;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::PollEvent(SDL_Event* event) {
	if (event->type != SDL_MOUSEWHEEL && event->type != SDL_MOUSEBUTTONDOWN && event->type != SDL_MOUSEBUTTONUP && event->type != SDL_TEXTINPUT && event->type != SDL_KEYDOWN && event->type != SDL_KEYUP && event->type != SDL_MOUSEMOTION)
		return;

	ImGuiIO& io = ImGui::GetIO();

	// Emulate these 2 events, because ImGui is broken... :c
	if (event->type == SDL_MOUSEMOTION) {
		io.MousePos.x += (float)event->motion.xrel;
		io.MousePos.y += (float)event->motion.yrel;
	} else if (event->type == SDL_MOUSEWHEEL) {
		io.MouseWheelH += (float)event->wheel.x;
		io.MouseWheel  += (float)event->wheel.y;
	} else
		ImGui_ImplSDL2_ProcessEvent(event);

	// Cancel both up and down events, but only down events do something
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
		if (event->key.keysym.sym == SDLK_INSERT || (event->key.keysym.mod == KMOD_LALT && event->key.keysym.sym == SDLK_i)) {
			if (event->type == SDL_KEYDOWN)
				visible = !visible;
			event->type = 0;
		} else if (visible && event->key.keysym.sym == SDLK_ESCAPE) {
			if (event->type == SDL_KEYDOWN)
				visible = false;
			event->type = 0;
		}
	}

	if (visible) {
		if (event->type == SDL_MOUSEBUTTONUP)
			reinterpret_cast<void (*)(SDL_Window*, int, int)>(Hooks::SDL::warpMouseInWindow->proxy)(Hooks::SDL::windowPtr, (int)io.MousePos.x, (int)io.MousePos.y);

		if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEMOTION /* this one might lead to desyncs */)
			event->type = 0;
	}
}

bool Gui::WarpMouseInWindow() {
	// lol no
	return visible;
}
