#ifndef FEATURES_LEGIT_AIMBOT
#define FEATURES_LEGIT_AIMBOT

#include "SDL.h"

#include "../../Serialization/Serializer.hpp"

namespace Features::Legit::Aimbot {
	// Settings
	extern bool enabled;
	extern float fov;
	extern float smoothness;
	extern int maximalInfluence;
	extern int maximalFlashAmount;
	extern bool dontAimThroughSmoke;
	extern bool friendlyFire;

	void PollEvent(SDL_Event* event);

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
