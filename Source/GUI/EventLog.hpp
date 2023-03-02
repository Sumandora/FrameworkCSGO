#ifndef GUI_EVENTLOG
#define GUI_EVENTLOG

#include "SDL.h"
#include "imgui.h"

namespace Gui::EventLog {
	void ImGuiRender(ImDrawList* drawList);
	void CreateReport(const char* fmt, ...);
}

#endif
