#ifndef FEATURES_GENERAL_MENU
#define FEATURES_GENERAL_MENU

#include "../Features.hpp"

#include "../../GUI/Elements/Keybind.hpp"

#include "imgui.h"
#include <optional>

inline class Menu : public Feature {
public:
	Menu()
		: Feature("General", "Menu")
	{
	}

	class MenuKey : public Input {
	public:
		inline MenuKey()
			: Input(Input::Method::KEY, Input::Type::TOGGLE, ImGuiKey_Insert, true){};
		virtual bool IsAllowed(Method method, Type type, std::optional<unsigned int> key);
		virtual void OnChange();
	} menuKey{};

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