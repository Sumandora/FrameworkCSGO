 #include "Settings.hpp"

#include "xorstr.hpp"

#include "Features/Features.hpp"
#include "GUI/ImGuiMacros.hpp"

void Gui::Construction::SetupConstruction()
{
	Features::SetupGUI();
	TABBAR(xorstr_("#Settings"), []() {
		TABITEM(xorstr_("Configuration"), Gui::Construction::SetupConfiguration);
		TABITEM(xorstr_("Debug"), Gui::Construction::SetupDebug);
	})
}