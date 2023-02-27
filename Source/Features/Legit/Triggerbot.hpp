#ifndef FEATURES_LEGIT_TRIGGERBOT
#define FEATURES_LEGIT_TRIGGERBOT

#include "../../SDK/CUserCmd.hpp"

#include "../../GUI/Elements/Keybind.hpp"

namespace Features {
	namespace Legit {
		namespace Triggerbot {
			// Settings
			extern bool enabled;
			extern int	input;

			void		CreateMove(CUserCmd* cmd);

			// GUI
			void		SetupGUI();
		}
	}
}

#endif
