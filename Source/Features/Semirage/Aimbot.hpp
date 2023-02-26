#ifndef FEATURES_SEMIRAGE_AIMBOT
#define FEATURES_SEMIRAGE_AIMBOT

#include "../../SDK/CUserCmd.hpp"

namespace Features {
	namespace Semirage {
		namespace Aimbot {
			// Settings
			extern bool		enabled;
			extern bool		onlyWhenShooting;
			extern float	fov;
			extern float	aimSpeed;
			extern bool		silent;
			extern float	snapBack;

			bool CreateMove(CUserCmd* cmd);

			// GUI
			void SetupGUI();
		}
	}
}

#endif
