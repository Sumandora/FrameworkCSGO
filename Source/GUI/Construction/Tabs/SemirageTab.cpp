#include "../Settings.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../ImGuiMacros.hpp"

#include "../../../Features/Semirage/Aimbot.hpp"
#include "../../../Features/Semirage/Backtrack.hpp"
#include "../../../Features/Semirage/RecoilAssistance.hpp"

void Gui::Construction::SetupSemirage()
{
	TABBAR(xorstr_("#Semirage settings"), []() {
		TABITEM(xorstr_("Aimbot"), Features::Semirage::Aimbot::SetupGUI);
		TABITEM(xorstr_("Recoil assistance"), Features::Semirage::RecoilAssistance::SetupGUI);
		TABITEM(xorstr_("Backtrack"), Features::Semirage::Backtrack::SetupGUI);
	})
}