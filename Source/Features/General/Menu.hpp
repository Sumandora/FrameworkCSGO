#ifndef FEATURES_GENERAL_MENU
#define FEATURES_GENERAL_MENU

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

inline class Menu : public Feature {
public:
	Menu()
		: Feature(xorstr_("General"), xorstr_("Menu"))
	{
	}

	int menuKey = ImGuiKey_Insert;
	int style = 0;

#ifdef DEBUG
	bool isShowingDemoWindow = false;
	bool isShowingMetricsWindow = false;
	bool isShowingDebugLogWindow = false;
	bool isShowingStackToolWindow = false;
	bool isShowingAboutWindow = false;
	bool isShowingStyleEditor = false;
	bool isShowingUserGuide = false;
#endif

	void ImGuiLoop();

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} menu;

#endif