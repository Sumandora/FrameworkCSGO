#ifndef FEATURES_MOVEMENT
#define FEATURES_MOVEMENT

#include "../SDK/GameClass/CUserCmd.hpp"

#include "../Serialization/Serializer.hpp"

// TODO More keybinds

namespace Features::Movement {
	namespace Bhop {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace CrouchJump {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace JumpBug {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace EdgeJump {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace AutoStrafer {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace FastDuck {
		void CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	// TODO Auto ladder
	// TODO Slow walk

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
