#ifndef FEATURES_SEMIRAGE_AIMBOT
#define FEATURES_SEMIRAGE_AIMBOT

#include "../../SDK/CUserCmd.hpp"

#include "../../Serialization/Serializer.hpp"

namespace Features::Semirage::Aimbot {
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

#endif
