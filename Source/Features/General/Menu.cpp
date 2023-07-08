#include "Menu.hpp"

#include "EventLog.hpp"

#include "../../Hooks/SDL/SDLFunctions.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../GUI/GUI.hpp"

#include "../../Utils/Platform/CompilerSupport.hpp"

static bool wasPressed = false;

void Menu::ImGuiLoop()
{
	switch (style) {
	case 0:
	default:
		ImGui::StyleColorsDark(&ImGui::GetStyle());
		break;
	case 1:
		ImGui::StyleColorsLight(&ImGui::GetStyle());
		break;
	case 2:
		ImGui::StyleColorsClassic(&ImGui::GetStyle());
		break;
	}

#ifdef DEBUG
	if (Gui::visible) {
		if (isShowingDemoWindow)
			ImGui::ShowDemoWindow();
		if (isShowingMetricsWindow)
			ImGui::ShowMetricsWindow();
		if (isShowingDebugLogWindow)
			ImGui::ShowDebugLogWindow();
		if (isShowingStackToolWindow)
			ImGui::ShowStackToolWindow();
#ifndef MENUPREVIEW
		if (isShowingAboutWindow)
			ImGui::ShowAboutWindow();
		if (isShowingStyleEditor)
			ImGui::ShowStyleEditor();
		if (isShowingUserGuide)
			ImGui::ShowUserGuide();
#endif
	}
#endif

	const bool isPressed = IsInputDown(menuKey, false, std::nullopt);

	if (isPressed && !wasPressed) {
		Gui::visible = !Gui::visible;
		eventLog.CreateReport("%s the menu", Gui::visible ? "Opened" : "Closed");
	}
	wasPressed = isPressed;
}

void Menu::SetupGUI()
{
	ImGui::InputSelector("Menu key (%s)", menuKey);
	if (menuKey == 0)
		menuKey = static_cast<int>(ImGuiKey_Insert);
#ifdef DEBUG
	ImGui::Checkbox("Show Demo Window", &isShowingDemoWindow);
	ImGui::Checkbox("Show Metrics Window", &isShowingMetricsWindow);
	ImGui::Checkbox("Show Debug Log Window", &isShowingDebugLogWindow);
	ImGui::Checkbox("Show Stack Tool Window", &isShowingStackToolWindow);
	ImGui::Checkbox("Show About Window", &isShowingAboutWindow);
	ImGui::Checkbox("Show Style Editor", &isShowingStyleEditor);
	ImGui::Checkbox("Show User Guide", &isShowingUserGuide);
#endif
	if (ImGui::Button("Set style to dark")) {
		style = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Set style to light")) {
		style = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button("Set style to classic")) {
		style = 2;
	}

	ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
	ImGui::Text("Compiled using: " COMPILER_NAME);
	ImGui::Text("Project name: " PROJECT_NAME);
}

SCOPED_SERIALIZER(Menu)
{
	SERIALIZE("Menu key", menuKey);
	SERIALIZE("Style", style);
	// Intentionally not saving the showing...window because nobody needs those
}
