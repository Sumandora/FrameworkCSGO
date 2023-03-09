#ifndef FEATURES_SEMIRAGE_BACKTRACK
#define FEATURES_SEMIRAGE_BACKTRACK

#include "imgui.h"

#include "../../SDK/ClientFrameStage.hpp"
#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Semirage::Backtrack {
	// Settings
	extern bool enabled;
	extern float timeDelta;

	void CreateMove(CUserCmd* cmd);
	void FrameStageNotify();

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
