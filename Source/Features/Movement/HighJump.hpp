#ifndef FEATURES_MOVEMENT_HIGHJUMP
#define FEATURES_MOVEMENT_HIGHJUMP

#include "../../SDK/CUserCmd.hpp"

namespace Features::Movement::HighJump {
	// Settings
	extern bool enabled;
	extern int input;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();
}

#endif
