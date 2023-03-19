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
	namespace CrouchJump {
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
	namespace EdgeJump {
		// Settings
		extern bool enabled;
		extern int input;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace AutoStrafer {
		// Settings
		extern bool enabled;
		extern bool directional;
		extern bool onlyWhenIdle;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace FastDuck {
		// Settings
		extern bool enabled;

		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	// TODO Auto Ladder
	// TODO Slow walk

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
