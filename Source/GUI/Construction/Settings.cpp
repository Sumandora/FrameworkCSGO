#include "Settings.hpp"

#include "xorstr.hpp"

#include "../ImGuiMacros.hpp"

void Gui::Construction::SetupConstruction() {
	TABBAR(xorstr_("#Settings"), []() {
		TABITEM(xorstr_("General"),		Gui::Construction::SetupGeneral);
		TABITEM(xorstr_("Legit"),		Gui::Construction::SetupLegit);
		TABITEM(xorstr_("Semirage"),	Gui::Construction::SetupSemirage);
		TABITEM(xorstr_("Movement"),	Gui::Construction::SetupMovement);
		TABITEM(xorstr_("Debug"),		Gui::Construction::SetupDebug);
	})
}