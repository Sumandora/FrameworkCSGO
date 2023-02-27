#ifndef FEATURES_LEGIT_SPECTATORLIST
#define FEATURES_LEGIT_SPECTATORLIST

#include "imgui.h"

namespace Features {
	namespace Legit {
		namespace SpectatorList {
			// Settings
			extern bool enabled;

			void		ImGuiRender(ImDrawList* drawList);

			// GUI
			void		SetupGUI();
		}
	}
}

#endif
