#include "../Settings.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../ImGuiMacros.hpp"

#include "../../../Features/Movement/Bhop.hpp"
#include "../../../Features/Movement/HighJump.hpp"
#include "../../../Features/Movement/JumpBug.hpp"

void Gui::Construction::SetupMovement()
{
	TABBAR(xorstr_("#Movement settings"), []() {
		TABITEM(xorstr_("Bhop"), Features::Movement::Bhop::SetupGUI);
		TABITEM(xorstr_("High jump"), Features::Movement::HighJump::SetupGUI);
		TABITEM(xorstr_("Jump bug"), Features::Movement::JumpBug::SetupGUI);
	})
}