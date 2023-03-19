#ifndef FEATURES_SEMIRAGE
#define FEATURES_SEMIRAGE

#include "../SDK/CUserCmd.hpp"

#include "../Serialization/Serializer.hpp"

namespace Features::Semirage {
	namespace Aimbot {
		// Settings
		extern bool enabled;
		extern bool onlyWhenShooting;
		extern float fov;
		extern float aimSpeed;
		extern int maximalFlashAmount;
		extern bool dontAimThroughSmoke;
		extern bool silent;
		extern float snapBack;
		extern bool friendlyFire;

		bool CreateMove(CUserCmd* cmd);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Backtrack {
		// Settings
		extern bool enabled;
		extern float time;
		extern bool accountForOutgoingPing;
		extern bool friendlyFire;

		void CreateMove(CUserCmd* cmd);
		void FrameStageNotify();

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace RecoilAssistance {
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

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
