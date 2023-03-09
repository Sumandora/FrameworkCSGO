#ifndef FEATURES_MOVEMENT_BHOP
#define FEATURES_MOVEMENT_BHOP

#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Movement::Bhop {
	// Settings
	extern bool enabled;
	extern int humanization;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
