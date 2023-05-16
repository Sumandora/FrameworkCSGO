#include "Features.hpp"

#include "GUI/ImGuiMacros.hpp"

void Features::General::SetupGUI()
{
	TABBAR(xorstr_("#General settings"), []() {
		TABITEM(xorstr_("Menu"), Menu::SetupGUI);
		TABITEM(xorstr_("Watermark"), Watermark::SetupGUI);
		TABITEM(xorstr_("Engine prediction"), EnginePrediction::SetupGUI);
		TABITEM(xorstr_("Event log"), EventLog::SetupGUI);
	})
}

void Features::Legit::SetupGUI()
{
	TABBAR(xorstr_("#Legit settings"), []() {
		TABITEM(xorstr_("Aimbot"), Aimbot::SetupGUI);
		TABITEM(xorstr_("Triggerbot"), Triggerbot::SetupGUI);
	})
}

void Features::Semirage::SetupGUI()
{
	TABBAR(xorstr_("#Semirage settings"), []() {
		TABITEM(xorstr_("Aimbot"), Aimbot::SetupGUI);
		TABITEM(xorstr_("Recoil assistance"), RecoilAssistance::SetupGUI);
		TABITEM(xorstr_("Backtrack"), Backtrack::SetupGUI);
	})
}

void Features::Movement::SetupGUI()
{
	TABBAR(xorstr_("#Movement settings"), []() {
		TABITEM(xorstr_("Bhop"), Bhop::SetupGUI);
		TABITEM(xorstr_("Crouch jump"), CrouchJump::SetupGUI);
		TABITEM(xorstr_("Jump bug"), JumpBug::SetupGUI);
		TABITEM(xorstr_("Edge jump"), EdgeJump::SetupGUI);
		TABITEM(xorstr_("Auto strafer"), AutoStrafer::SetupGUI);
		TABITEM(xorstr_("Fast duck"), FastDuck::SetupGUI);
	})
}

void Features::Visuals::SetupGUI()
{
	TABBAR(xorstr_("#Visuals settings"), []() {
		TABITEM(xorstr_("ESP"), Esp::SetupGUI);
		TABITEM(xorstr_("Spectator List"), SpectatorList::SetupGUI);
		TABITEM(xorstr_("No punch"), NoPunch::SetupGUI);
		TABITEM(xorstr_("FOV"), FOV::SetupGUI);
		TABITEM(xorstr_("Flash reduction"), FlashReduction::SetupGUI);
	})
}

void Features::SetupGUI()
{
	TABBAR(xorstr_("#Settings"), []() {
		TABITEM(xorstr_("General"), General::SetupGUI);
		TABITEM(xorstr_("Legit"), Legit::SetupGUI);
		TABITEM(xorstr_("Semirage"), Semirage::SetupGUI);
		TABITEM(xorstr_("Movement"), Movement::SetupGUI);
		TABITEM(xorstr_("Visuals"), Visuals::SetupGUI);
	})
}
