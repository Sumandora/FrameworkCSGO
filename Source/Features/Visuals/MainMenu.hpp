#ifndef FEATURES_VISUALS_MAINMENU
#define FEATURES_VISUALS_MAINMENU

#include "../Features.hpp"

#include "imgui.h"

#include "../../GUI/ImGuiColors.hpp"
#include <map>
#include <utility>

inline class MainMenu : public Feature {
public:
	MainMenu()
		: Feature("Visuals", "Main menu")
	{
	}
	~MainMenu();

	std::map<std::pair<const char*, const char*>, bool> panoramaWidgets{
		{ { "Show news", "JsNewsContainer" }, true },
		{ { "Show player", "JsMainmenu_Vanity" }, true },
		{ { "Show player settings", "VanityControls" }, true },
		{ { "Show background", "MainMenuMovieParent" }, true },
		{ { "Show nav bar", "MainMenuNavBarLeft" }, true },
		{ { "Show sidebar", "MainMenuFriendsAndParty" }, true }
	};

	std::map<std::pair<const char*, const char*>, bool> navBarButtons{
		{ { "Show server browser button", "MainMenuNavBarShowCommunityServerBrowser" }, false },
		{ { "Show tournament watch button", "MainMenuNavBarWatch" }, true },
		{ { "Show stats button", "MainMenuNavBarStats" }, true }
	};

private:
	void UpdateVisiblityNow();

public:
	void UpdateVisibility();

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} mainMenu;

#endif