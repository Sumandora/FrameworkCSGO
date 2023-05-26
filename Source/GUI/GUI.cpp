#include "GUI.hpp"

#include <algorithm>
#include <mutex>

#include "imgui.h"
#include "xorstr.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

#include "../Hooks/SDL/SDLFunctions.hpp"

#include "../Features/General/General.hpp"
#include "../Features/Semirage/Semirage.hpp"
#include "../Features/Visuals/Visuals.hpp"

#include "Construction/Settings.hpp"

bool Gui::visible = true;

void Gui::Create()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
}

void Gui::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Gui::BuildMenu(int width, int height)
{
	const ImVec2 size(800, 600);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2((float)width * 0.1f, (float)height * 0.1f), ImGuiCond_Once);

	ImGui::Begin(xorstr_("Framework"));

	ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Once);

	Construction::SetupConstruction();

	ImGui::End();
}

void Gui::SwapWindow(SDL_Window* window)
{
	ImGuiIO& io = ImGui::GetIO();

	static std::once_flag init;
	std::call_once(init, [&io, window]() {
		ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
		ImGui_ImplOpenGL3_Init();

		// We are running straight into the multi monitor dpi issue here, but to my knowledge there is no appropriate solution to this when using ImGui
		SDL_DisplayMode mode;
		const int displayIndex = SDL_GetWindowDisplayIndex(window);
		SDL_GetCurrentDisplayMode(displayIndex, &mode);

		// I want the font size to be something around 12 on full hd screens
		// 4k screens get something around 24
		// Mathematically expressed:
		// fontSize(screenHeight) = screenHeight * x
		// fontSize(1080) = 12
		// fontSize(2160) = 24
		// x = 1/90, also technically one pair of values is enough here to solve for x

		const float fontSize = floor(mode.h * (1.0f / 90.0f));

		// Might not work on certain distros/configurations
		bool loadedFont = false;

		for (const char* path : {
				 xorstr_("/usr/share/fonts/noto/NotoSans-Regular.ttf"),
				 xorstr_("/usr/share/fonts/google-noto/NotoSans-Regular.ttf") }) {
			if (access(path, F_OK) == 0 && io.Fonts->AddFontFromFileTTF(path, fontSize)) {
				loadedFont = true;
				break;
			}
		}

		if (!loadedFont) {
			ImFontConfig config;
			config.SizePixels = fontSize;
			io.Fonts->AddFontDefault(&config);
		}
	});

	io.SetPlatformImeDataFn = nullptr;

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	io.DisplaySize = ImVec2((float)width, (float)height);
	io.MousePos = ImVec2(std::clamp((float)io.MousePos.x, 0.0f, (float)width), std::clamp((float)io.MousePos.y, 0.0f, (float)height));

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	Features::General::Menu::ImGuiLoop(); // Will take care of the menu key

	if (visible) {
		BuildMenu(width, height);
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
static Uint32 lastTextInput;

void Gui::PollEvent(SDL_Event* event)
{
	if (event->type == SDL_TEXTINPUT && lastTextInput >= event->text.timestamp)
		return;

	ImGuiIO& io = ImGui::GetIO();
	// Emulate these 2 events, because ImGui is broken... :c
	if (event->type == SDL_MOUSEMOTION) {
		int x, y;
		SDL_GetMouseState(&x, &y);

		io.MousePos.x = (float)x;
		io.MousePos.y = (float)y;
		if(visible && SDL_GetMouseFocus() == Hooks::SDL::windowPtr)
			reinterpret_cast<void (*)(SDL_Window*, int, int)>(Hooks::SDL::WarpMouseInWindow::hook->proxy)(Hooks::SDL::windowPtr, (int)io.MousePos.x, (int)io.MousePos.y);
	} else if (event->type == SDL_MOUSEWHEEL) {
		io.MouseWheelH += (float)event->wheel.x;
		io.MouseWheel += (float)event->wheel.y;
	} else
		ImGui_ImplSDL2_ProcessEvent(event);

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
