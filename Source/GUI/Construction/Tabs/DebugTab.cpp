#include "DebugTab.hpp"
#include "../Settings.hpp"

#include "xorstr.hpp"

#include "../../ImGuiMacros.hpp"

void Gui::Construction::SetupDebug()
{
	TABBAR(xorstr_("#Debug settings"), []() {
		TABITEM(xorstr_("Netvars"), Debug::NetvarsTab);
		TABITEM(xorstr_("Interfaces"), Debug::InterfacesTab);
	})
}