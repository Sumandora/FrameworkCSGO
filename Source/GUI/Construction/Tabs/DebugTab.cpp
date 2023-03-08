#include "../Settings.hpp"

#include "xorstr.hpp"
#include "imgui.h"

#include "../../ImGuiMacros.hpp"

#include "../../../Netvars.hpp"
#include "../../../Interfaces.hpp"

void Gui::Construction::SetupDebug() {
	TABBAR(xorstr_("#Debug settings"), []() {
		TABITEM(xorstr_("Netvars"),		Netvars::SetupGUI);
		TABITEM(xorstr_("Interfaces"),	Interfaces::SetupGUI);
	})
}