#include "MainMenu.hpp"

#include "../../Hooks/SDL/SDLFunctions.hpp"
#include "../../Interfaces.hpp"

#include "imgui.h"

#include <dlfcn.h>
#include <ios>
#include <sstream>
#include <string>

static void setVisible(const char* item, bool visible)
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
		setVisible(widget.first.second, true);

	for (auto& button : navBarButtons)
		setVisible(button.first.second, strcmp(button.first.second, "MainMenuNavBarShowCommunityServerBrowser") != 0);
}

void MainMenu::updateVisiblityNow() const // TODO Don't call IsInGame twice when coming from UpdateVisibility
{
	if (!Interfaces::engine->IsInGame())
		for (auto& widget : panoramaWidgets)
			setVisible(widget.first.second, widget.second);

	// In case the user changes nav bar buttons, we at least try. We expect an error, but who knows...
	for (auto& button : navBarButtons)
		setVisible(button.first.second, button.second);
}

void MainMenu::updateVisibility() const
{
	if (!Interfaces::engine->IsInGame())
		return;

	static float lastUpdate = 0.0f;
	if (Memory::globalVars->curtime - lastUpdate > 1.0f) {
		updateVisiblityNow();
		lastUpdate = Memory::globalVars->curtime;
	}
}

void MainMenu::setupGUI()
{
	ImGui::Separator();
	for (auto& widget : panoramaWidgets)
		if (ImGui::Checkbox(widget.first.first, &widget.second))
			updateVisiblityNow();
	ImGui::Separator();
	for (auto& button : navBarButtons)
		if (ImGui::Checkbox(button.first.first, &button.second))
			updateVisiblityNow();
}

SCOPED_SERIALIZER(MainMenu)
{
	for (auto& widget : panoramaWidgets)
		SERIALIZE(widget.first.first, widget.second);
	for (auto& button : navBarButtons)
		SERIALIZE(button.first.first, button.second);
	if (direction == Direction::DESERIALIZE)
		updateVisiblityNow();
}
