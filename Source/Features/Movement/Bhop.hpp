#ifndef FEATURES_MOVEMENT_BHOP
#define FEATURES_MOVEMENT_BHOP

#include "../../SDK/CUserCmd.hpp"

namespace Features::Movement::Bhop {
	// Settings
	extern bool enabled;
	extern int	humanization;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();
}

#endif
