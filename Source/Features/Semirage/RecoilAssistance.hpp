#ifndef FEATURES_SEMIRAGE_RECOILASSISTANCE
#define FEATURES_SEMIRAGE_RECOILASSISTANCE

#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Semirage::RecoilAssistance {
	// Settings
	extern bool enabled;
	extern float horizontalAdjustment;
	extern float verticalAdjustment;
	extern bool silent;
	extern int minShots;

	bool CreateMove(CUserCmd* cmd);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
