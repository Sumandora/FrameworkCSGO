#ifndef FEATURES_GENERAL_WATERMARK
#define FEATURES_GENERAL_WATERMARK

#include "imgui.h"

namespace Features::General::Watermark {
	// Settings
	extern bool enabled;

	void ImGuiRender(ImDrawList* drawList);

	// GUI
	void SetupGUI();
}

#endif
