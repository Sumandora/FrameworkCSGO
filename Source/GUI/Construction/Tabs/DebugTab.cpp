#include "DebugTab.hpp"

#include "xorstr.hpp"

#include "GUI/Construction/Settings.hpp"
#include "GUI/ImGuiMacros.hpp"

void Gui::Construction::SetupDebug()
{
	TABBAR(xorstr_("#Debug settings"), []() {
		TABITEM(xorstr_("Netvars"), Debug::NetvarsTab);
		TABITEM(xorstr_("Interfaces"), Debug::InterfacesTab);
	})
}