#ifndef FEATURES_LEGIT_ESP
#define FEATURES_LEGIT_ESP

#include "imgui.h"

struct ImDrawList;

namespace Features {
	namespace Legit {
		namespace Esp {
			// Settings
			extern bool		enabled;
			extern float	rounding;
			extern float	thickness;
			extern bool		outlined;
			extern float	outlineThickness;

			void ImGuiRender(ImDrawList* drawList);

			// GUI
			void SetupGUI();
		}
	}
}

#endif
