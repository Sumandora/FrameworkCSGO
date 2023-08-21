#include "Menu.hpp"

#include "EventLog.hpp"

#include "../../Hooks/SDL/SDLFunctions.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../GUI/GUI.hpp"

#include "../../Utils/Platform/CompilerSupport.hpp"
#include "imgui.h"
#include <optional>

bool Menu::MenuKey::isAllowed(Method method, Type type, std::optional<unsigned int> key)
{
	if (type == Input::Type::HOLD)
		return false; // I mean, this is technically not impossible, but honestly who would want this?
	if (method == Input::Method::MOUSE && key == ImGuiMouseButton_Left)
		return false; // This would result in the menu opening/closing when you try to change the key... completely useless
	return key.has_value(); // This would render the menu unusable!
}

void Menu::MenuKey::onChange()
{
	Gui::visible = !Gui::visible;
	eventLog.createReport("%s the menu", Gui::visible ? "Opened" : "Closed");
}

void Menu::imGuiLoop()
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
}

void Menu::setupGUI()
{
	ImGui::InputSelector("Menu key (%s)", menuKey);
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
	SERIALIZE_STRUCT("Menu key", menuKey);
	SERIALIZE("Style", style);
	// Intentionally not saving the showing...window because nobody needs those
}
