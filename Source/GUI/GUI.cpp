#include "GUI.hpp"

#include <algorithm>
#include <mutex>

#include "imgui.h"
#include "xorstr.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

#include "../Hooks/SDL/SDLHook.hpp"

#include "../Features/General.hpp"
#include "../Features/Semirage.hpp"
#include "../Features/Visuals.hpp"

#include "Construction/Settings.hpp"

bool Gui::visible = true;

// TODO More aggressive mouse syncing

void Gui::Create()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	const float fontSize = 24.0f;
	// Might not work on certain distros/configurations
	if (!io.Fonts->AddFontFromFileTTF(xorstr_("/usr/share/fonts/noto/NotoSans-Regular.ttf"), fontSize))
		io.Fonts->AddFontDefault();

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
}

void Gui::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Gui::SwapWindow(SDL_Window* window)
{
	ImGuiIO& io = ImGui::GetIO();

	static std::once_flag init;
	std::call_once(init, [&]() {
		ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
		ImGui_ImplOpenGL3_Init();
	});

	io.SetPlatformImeDataFn = nullptr;

	int width {}, height {};
	SDL_GetWindowSize(window, &width, &height);

	io.DisplaySize = ImVec2((float)width, (float)height);
	io.MousePos = ImVec2(std::clamp((float)io.MousePos.x, 0.0f, (float)width), std::clamp((float)io.MousePos.y, 0.0f, (float)height));

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	Features::General::Menu::ImGuiLoop(); // Will take care of the menu key

	if (visible) {
		const ImVec2 size(800, 600);
		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2((float)width * 0.1f, (float)height * 0.1f), ImGuiCond_Once);

		ImGui::Begin(xorstr_("Framework"));

		ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Once);

		Construction::SetupConstruction();

		ImGui::End();
	}

	Features::General::Watermark::ImGuiRender(ImGui::GetBackgroundDrawList());
	Features::General::EventLog::ImGuiRender(ImGui::GetBackgroundDrawList());

	Features::Semirage::Aimbot::ImGuiRender(ImGui::GetBackgroundDrawList());
	Features::Semirage::Backtrack::ImGuiRender(ImGui::GetBackgroundDrawList());

	Features::Visuals::Esp::ImGuiRender(ImGui::GetBackgroundDrawList());
	Features::Visuals::SpectatorList::ImGuiRender(ImGui::GetBackgroundDrawList());

	io.MouseDrawCursor = visible;
	io.WantCaptureMouse = visible;
	io.WantCaptureKeyboard = visible;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// I don't even want to know why I have to do this
static unsigned int lastTextInput;

void Gui::PollEvent(SDL_Event* event)
{
	if (event->type == SDL_TEXTINPUT && lastTextInput >= event->text.timestamp)
		return;

	ImGuiIO& io = ImGui::GetIO();
	// Emulate these 2 events, because ImGui is broken... :c
	if (event->type == SDL_MOUSEMOTION) {
		io.MousePos.x += (float)event->motion.xrel;
		io.MousePos.y += (float)event->motion.yrel;
	} else if (event->type == SDL_MOUSEWHEEL) {
		io.MouseWheelH += (float)event->wheel.x;
		io.MouseWheel += (float)event->wheel.y;
	} else
		ImGui_ImplSDL2_ProcessEvent(event);

	if (event->type == SDL_MOUSEBUTTONUP)
		reinterpret_cast<void (*)(SDL_Window*, int, int)>(Hooks::SDL::warpMouseInWindow->proxy)(Hooks::SDL::windowPtr, (int)io.MousePos.x, (int)io.MousePos.y);

	if (event->type == SDL_TEXTINPUT)
		lastTextInput = event->text.timestamp;

	if (visible)
		event->type = -1; // Change type to an invalid event to make the game ignore it.
}

bool Gui::WarpMouseInWindow()
{
	// lol no
	return visible;
}
