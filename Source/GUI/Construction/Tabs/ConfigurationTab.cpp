#include "ConfigurationTab.hpp"
#include "../Settings.hpp"

#include "xorstr.hpp"

#include "../../ImGuiMacros.hpp"

void Gui::Construction::SetupConfiguration()
{
	TABBAR(xorstr_("#Configuration settings"), []() {
		TABITEM(xorstr_("Save/Load"), Configuration::SaveLoadTab);
	})
}