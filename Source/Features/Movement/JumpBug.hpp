#ifndef FEATURES_MOVEMENT_JUMPBUG
#define FEATURES_MOVEMENT_JUMPBUG

#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Movement::JumpBug {
	// Settings
	extern bool enabled;
	extern int input;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
