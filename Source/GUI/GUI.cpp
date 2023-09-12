#include "GUI.hpp"

#include <algorithm>
#include <cmath>
#include <mutex>

#include "Elements/Keybind.hpp"
#include "imgui.h"
#include "ImGuiColors.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"

#include "../Features/General/EventLog.hpp"
#include "../Features/General/Menu.hpp"
#include "../Features/General/Watermark.hpp"

#include "../Features/Semirage/Aimbot.hpp"
#include "../Features/Semirage/Backtrack.hpp"

#include "../Features/Visuals/ESP/ESP.hpp"
#include "../Features/Visuals/SpectatorList.hpp"

bool Gui::visible = true;

void Gui::create()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = nullptr; // TODO Bring it back
	io.LogFilename = nullptr;
}

void Gui::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Gui::swapWindow(SDL_Window* window)
{
	ImGuiIO& io = ImGui::GetIO();

	static std::once_flag init;
	std::call_once(init, [&io, window]() {
		ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
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

		const float fontSize = floorf((float)mode.h * (1.0f / 90.0f));

		// Might not work on certain distros/configurations
		bool loadedFont = false;

		for (const char* path : {
				 "/usr/share/fonts/noto/NotoSans-Regular.ttf",
				 "/usr/share/fonts/google-noto/NotoSans-Regular.ttf" }) {
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

	for (Input* input : inputs)
		input->updateState();

	menu.imGuiLoop(); // Will take care of the menu key

	if (visible) {
		build();
	}

	watermark.imGuiRender(ImGui::GetBackgroundDrawList());
	eventLog.imGuiRender(ImGui::GetBackgroundDrawList());

	semirageAimbot.imGuiRender(ImGui::GetBackgroundDrawList());
	backtrack.imGuiRender(ImGui::GetBackgroundDrawList());

	esp.imGuiRender(ImGui::GetBackgroundDrawList());
	spectatorList.imGuiRender(ImGui::GetBackgroundDrawList());

	ImGui::GetForegroundDrawList()->AddRectFilled({ io.MousePos.x - 5, io.MousePos.y - 5 }, { io.MousePos.x + 5, io.MousePos.y + 5 }, ImGuiColors::red);

	io.MouseDrawCursor = visible;
	io.WantCaptureMouse = visible;
	io.WantCaptureKeyboard = visible;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Gui::pollEvent(SDL_Event* event)
{
	static Uint32 lastEvent;
	if (event->type == SDL_TEXTINPUT) {
		if (event->text.timestamp <= lastEvent)
			return visible; // Not quite sure, why I have to do this
		lastEvent = event->text.timestamp;
	}

#if SDL_VERSION_ATLEAST(2, 0, 18)
	// CS:GOs SDL is 2.0.15, however we can't realistically expect users to download this ancient version, so just do this
	// preciseX/Y got introduced in 2.0.18
	if (event->type == SDL_MOUSEWHEEL) {
		SDL_MouseWheelEvent sdlMouseWheelEvent{};
		sdlMouseWheelEvent.type = SDL_MOUSEWHEEL;
		sdlMouseWheelEvent.preciseX = (float)event->wheel.x;
		sdlMouseWheelEvent.preciseY = (float)event->wheel.y;
		SDL_Event newEvent;
		newEvent.wheel = sdlMouseWheelEvent;
		ImGui_ImplSDL2_ProcessEvent(&newEvent);
	} else
#endif
		ImGui_ImplSDL2_ProcessEvent(event);

	return visible;
}

bool Gui::warpMouseInWindow(int x, int y)
{
	if (!visible) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = (float)x;
		io.MousePos.y = (float)y;
	}
	return visible;
}
