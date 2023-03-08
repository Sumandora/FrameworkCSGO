#include "../Settings.hpp"

#include "xorstr.hpp"
#include "imgui.h"

#include "../../ImGuiMacros.hpp"

#include "../../../Features/General/Watermark.hpp"
#include "../../../Features/General/EnginePrediction.hpp"
#include "../../../Features/General/EventLog.hpp"

void Gui::Construction::SetupGeneral() {
	TABBAR(xorstr_("#General settings"), []() {
		TABITEM(xorstr_("Watermark"),			Features::General::Watermark::SetupGUI);
		TABITEM(xorstr_("Engine prediction"),	Features::General::EnginePrediction::SetupGUI);
		TABITEM(xorstr_("Event log"),			Features::General::EventLog::SetupGUI);
	})
}