#include "General.hpp"

#include "../../Hooks/SDL/SDLFunctions.hpp"

#include "xorstr.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../GUI/GUI.hpp"

#include "../../Utils/Platform/CompilerSupport.hpp"

static int menuKey = ImGuiKey_Insert;
static int style = 0;

#ifdef DEBUG
static bool isShowingDemoWindow = false;
static bool isShowingMetricsWindow = false;
static bool isShowingDebugLogWindow = false;
static bool isShowingStackToolWindow = false;
static bool isShowingAboutWindow = false;
static bool isShowingStyleEditor = false;
static bool isShowingUserGuide = false;
#endif

bool wasPressed = false;

void Features::General::Menu::ImGuiLoop()
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

	const bool isPressed = IsInputDown(menuKey, false, false);

	if (isPressed && !wasPressed) {
		Gui::visible = !Gui::visible;
		Features::General::EventLog::CreateReport(xorstr_("%s the menu"), Gui::visible ? xorstr_("Opened") : xorstr_("Closed"));
	}
	wasPressed = isPressed;
}

void Features::General::Menu::SetupGUI()
{
	ImGui::InputSelector(xorstr_("Menu key (%s)"), menuKey);
	if (menuKey == 0)
		menuKey = static_cast<int>(ImGuiKey_Insert);
#ifdef DEBUG
	ImGui::Checkbox(xorstr_("Show Demo Window"), &isShowingDemoWindow);
	ImGui::Checkbox(xorstr_("Show Metrics Window"), &isShowingMetricsWindow);
	ImGui::Checkbox(xorstr_("Show Debug Log Window"), &isShowingDebugLogWindow);
	ImGui::Checkbox(xorstr_("Show Stack Tool Window"), &isShowingStackToolWindow);
	ImGui::Checkbox(xorstr_("Show About Window"), &isShowingAboutWindow);
	ImGui::Checkbox(xorstr_("Show Style Editor"), &isShowingStyleEditor);
	ImGui::Checkbox(xorstr_("Show User Guide"), &isShowingUserGuide);
#endif
	if (ImGui::Button(xorstr_("Set style to dark"))) {
		style = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button(xorstr_("Set style to light"))) {
		style = 1;
	}
	ImGui::SameLine();
	if (ImGui::Button(xorstr_("Set style to classic"))) {
		style = 2;
	}

	ImGui::Text(xorstr_("ImGui Version: %s"), ImGui::GetVersion());
	ImGui::Text(xorstr_("Compiled using: " COMPILER_NAME));
}

BEGIN_SERIALIZED_STRUCT(Features::General::Menu::Serializer)
SERIALIZED_TYPE(xorstr_("Menu key"), menuKey)
// Intentionally not saving the showing...window because nobody needs those
SERIALIZED_TYPE(xorstr_("Style"), style)
END_SERIALIZED_STRUCT
