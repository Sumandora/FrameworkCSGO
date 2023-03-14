#ifndef FEATURES_MOVEMENT
#define FEATURES_MOVEMENT

#include "../SDK/CUserCmd.hpp"

#include "../Serialization/Serializer.hpp"

namespace Features::Movement {
	namespace Bhop {
		// Settings
		extern bool enabled;
		extern int humanization;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace HighJump {
		// Settings
		extern bool enabled;
		extern int input;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace JumpBug {
		// Settings
		extern bool enabled;
		extern int input;
		extern bool preDuck;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
