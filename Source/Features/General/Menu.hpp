#ifndef FEATURES_GENERAL_MENU
#define FEATURES_GENERAL_MENU

#include "imgui.h"
#include "SDL.h"

#include "../../Serialization/Serializer.hpp"

namespace Features::General::Menu {
	// Settings
	extern int menuKey;

	void ImGuiLoop();

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
