#include "../Settings.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../ImGuiMacros.hpp"

#include "../../../Features/Legit/Aimbot.hpp"
#include "../../../Features/Legit/ESP/ESP.hpp"
#include "../../../Features/Legit/SpectatorList.hpp"
#include "../../../Features/Legit/Triggerbot.hpp"

void Gui::Construction::SetupLegit()
{
	TABBAR(xorstr_("#Legit settings"), []() {
		TABITEM(xorstr_("Aimbot"), Features::Legit::Aimbot::SetupGUI);
		TABITEM(xorstr_("ESP"), Features::Legit::Esp::SetupGUI);
		TABITEM(xorstr_("Triggerbot"), Features::Legit::Triggerbot::SetupGUI);
		TABITEM(xorstr_("Spectator List"), Features::Legit::SpectatorList::SetupGUI);
	})
}