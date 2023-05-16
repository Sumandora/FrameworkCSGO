#ifndef FEATURES_GENERAL
#define FEATURES_GENERAL

#include "SDK/GameClass/CMoveData.hpp"
#include "SDK/GameClass/CUserCmd.hpp"
#include "SDK/MoveType.hpp"

#include "Serialization/Serializer.hpp"

namespace Features::General {
	namespace Menu {
		void ImGuiLoop();

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
#
	namespace Watermark {
		// Settings
		extern bool enabled;

		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace EnginePrediction {
		// Settings
		extern bool enabled;

		extern CMoveData moveData;
		extern int prePredictionFlags;
		extern MoveType prePredictionMoveType;
		void ImGuiWarning();

		void StartPrediction(CUserCmd* cmd);
		void EndPrediction();

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace EventLog {
		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// API
		void CreateReport(const char* fmt, ...);

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
