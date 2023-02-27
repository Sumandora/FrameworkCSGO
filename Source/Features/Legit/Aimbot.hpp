#ifndef FEATURES_LEGIT_AIMBOT
#define FEATURES_LEGIT_AIMBOT

#include "SDL.h"

namespace Features::Legit::Aimbot {
	// Settings
	extern bool	 enabled;
	extern float fov;
	extern float smoothness;
	extern int	 clamp;

	void		 PollEvent(SDL_Event* event);

	// GUI
	void		 SetupGUI();
}

#endif
