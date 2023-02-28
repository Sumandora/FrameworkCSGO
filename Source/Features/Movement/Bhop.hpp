#ifndef FEATURES_LEGIT_BHOP
#define FEATURES_LEGIT_BHOP

#include "../../SDK/CUserCmd.hpp"

namespace Features::Legit::Bhop {
	// Settings
	extern bool enabled;
	extern int	humanization;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();
}

#endif
