#ifndef FEATURES_LEGIT_TRIGGERBOT
#define FEATURES_LEGIT_TRIGGERBOT

#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Legit::Triggerbot {
	// Settings
	extern bool enabled;
	extern int input;
	extern bool secondaryFireWithR8Revolver;
	extern bool friendlyFire;

	void CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
