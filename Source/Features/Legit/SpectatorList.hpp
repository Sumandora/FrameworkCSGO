#ifndef FEATURES_LEGIT_SPECTATORLIST
#define FEATURES_LEGIT_SPECTATORLIST

#include "imgui.h"

#include "../../Serialization/Serializer.hpp"

namespace Features::Legit::SpectatorList {
	// Settings
	extern bool enabled;

	void ImGuiRender(ImDrawList* drawList);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
