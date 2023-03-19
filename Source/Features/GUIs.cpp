#include "Features.hpp"

#include "../GUI/ImGuiMacros.hpp"
#include "../Serialization/Serialization.hpp"

void Features::General::SetupGUI()
{
	TABBAR(xorstr_("#General settings"), []() {
		TABITEM(xorstr_("Menu"), Features::General::Menu::SetupGUI);
		TABITEM(xorstr_("Watermark"), Features::General::Watermark::SetupGUI);
		TABITEM(xorstr_("Engine prediction"), Features::General::EnginePrediction::SetupGUI);
		TABITEM(xorstr_("Event log"), Features::General::EventLog::SetupGUI);
		TABITEM(xorstr_("Configurations"), Serialization::SetupGUI);
	})
}

void Features::Legit::SetupGUI()
{
	TABBAR(xorstr_("#Legit settings"), []() {
		TABITEM(xorstr_("Aimbot"), Features::Legit::Aimbot::SetupGUI);
		TABITEM(xorstr_("ESP"), Features::Legit::Esp::SetupGUI);
		TABITEM(xorstr_("Triggerbot"), Features::Legit::Triggerbot::SetupGUI);
		TABITEM(xorstr_("Spectator List"), Features::Legit::SpectatorList::SetupGUI);
	})
}

void Features::Semirage::SetupGUI()
{
	TABBAR(xorstr_("#Semirage settings"), []() {
		TABITEM(xorstr_("Aimbot"), Features::Semirage::Aimbot::SetupGUI);
		TABITEM(xorstr_("Recoil assistance"), Features::Semirage::RecoilAssistance::SetupGUI);
		TABITEM(xorstr_("Backtrack"), Features::Semirage::Backtrack::SetupGUI);
	})
}

void Features::Movement::SetupGUI()
{
	TABBAR(xorstr_("#Movement settings"), []() {
		TABITEM(xorstr_("Bhop"), Features::Movement::Bhop::SetupGUI);
		TABITEM(xorstr_("Crouch jump"), Features::Movement::CrouchJump::SetupGUI);
		TABITEM(xorstr_("Jump bug"), Features::Movement::JumpBug::SetupGUI);
		TABITEM(xorstr_("Edge bug"), Features::Movement::EdgeJump::SetupGUI);
		TABITEM(xorstr_("Auto strafer"), Features::Movement::AutoStrafer::SetupGUI);
	})
}

void Features::SetupGUI()
{
	TABBAR(xorstr_("#Settings"), []() {
		TABITEM(xorstr_("General"), Features::General::SetupGUI);
		TABITEM(xorstr_("Legit"), Features::Legit::SetupGUI);
		TABITEM(xorstr_("Semirage"), Features::Semirage::SetupGUI);
		TABITEM(xorstr_("Movement"), Features::Movement::SetupGUI);
	})
}
