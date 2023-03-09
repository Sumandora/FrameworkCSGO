#ifndef FEATURES_GENERAL_ENGINEPREDICTION
#define FEATURES_GENERAL_ENGINEPREDICTION

#include "../../SDK/CUserCmd.hpp"
#include "../../SDK/GameClass/CMoveData.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::General::EnginePrediction {
	// Settings
	extern bool enabled;
	extern CMoveData moveData;

	void StartPrediction(CUserCmd* cmd);
	void EndPrediction();

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
