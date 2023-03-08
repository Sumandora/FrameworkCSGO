#include "../Settings.hpp"

#include "xorstr.hpp"
#include "imgui.h"

#include "../../ImGuiMacros.hpp"

#include "../../../Features/Movement/Bhop.hpp"
#include "../../../Features/Movement/HighJump.hpp"

void Gui::Construction::SetupMovement() {
	TABBAR(xorstr_("#Movement settings"), []() {
		TABITEM(xorstr_("Bhop"),				Features::Movement::Bhop::SetupGUI);
		TABITEM(xorstr_("High jump"),			Features::Movement::HighJump::SetupGUI);
	})
}