#ifndef FEATURES_LEGIT_ESP
#define FEATURES_LEGIT_ESP

#include <imgui.h>

namespace Features {
	namespace Legit {
		namespace Esp {
			// Settings
			extern bool enabled;
		
			void ImGuiRender(ImDrawList* drawList);

			// GUI
			void SetupGUI();
		}
	}
}

#endif
