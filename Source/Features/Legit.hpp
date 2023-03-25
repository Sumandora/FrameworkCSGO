#ifndef FEATURES_LEGIT
#define FEATURES_LEGIT

#include "../SDK/GameClass/CUserCmd.hpp"
#include "../Serialization/Serializer.hpp"
#include "SDL.h"

namespace Features::Legit {
	namespace Aimbot {
		void PollEvent(SDL_Event* event);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Triggerbot {
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
