#include "MainMenu.hpp"

#include "../../Hooks/SDL/SDLFunctions.hpp"
#include "../../Interfaces.hpp"

#include "imgui.h"

#include <dlfcn.h>
#include <ios>
#include <sstream>
#include <string>

static void SetVisible(const char* item, bool visible)
{
	std::stringstream ss{};
	ss << "var obj = $.GetContextPanel().FindChildTraverse(\"" << item << "\");";
	ss << "if(obj != null) obj.visible = " << std::boolalpha << visible << std::noboolalpha << ";";
	std::string str = ss.str();
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", str.c_str());
}

MainMenu::~MainMenu()
{
	if (Hooks::SDL::shuttingDown)
		return;
	// TODO maybe save defaults and use that instead, for scalability?
	for (auto& widget : panoramaWidgets)
		SetVisible(widget.first.second, true);

	for (auto& button : navBarButtons)
		SetVisible(button.first.second, strcmp(button.first.second, "MainMenuNavBarShowCommunityServerBrowser") != 0);
}

void MainMenu::UpdateVisiblityNow()
{
	if (!Interfaces::engine->IsInGame())
		for (auto& widget : panoramaWidgets)
			SetVisible(widget.first.second, widget.second);

	// In case the user changes nav bar buttons, we at least try. We expect an error, but who knows...
	for (auto& button : navBarButtons)
		SetVisible(button.first.second, button.second);
}

void MainMenu::UpdateVisibility()
{
	if (!Interfaces::engine->IsInGame())
		return;

	static float lastUpdate = 0.0f;
	if (Memory::globalVars->curtime - lastUpdate > 1.0f) {
		UpdateVisiblityNow();
		lastUpdate = Memory::globalVars->curtime;
	}
}

void MainMenu::SetupGUI()
{
	ImGui::Separator();
	for (auto& widget : panoramaWidgets)
		if (ImGui::Checkbox(widget.first.first, &widget.second))
			UpdateVisiblityNow();
	ImGui::Separator();
	for (auto& button : navBarButtons)
		if (ImGui::Checkbox(button.first.first, &button.second))
			UpdateVisiblityNow();
}

SCOPED_SERIALIZER(MainMenu)
{
	for (auto& widget : panoramaWidgets)
		SERIALIZE(widget.first.first, widget.second);
	for (auto& button : navBarButtons)
		SERIALIZE(button.first.first, button.second);
	UpdateVisiblityNow();
}
