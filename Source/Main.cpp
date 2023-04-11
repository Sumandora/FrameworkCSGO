#include <sys/stat.h>
#include <thread>

#include "xorstr.hpp"

#include "GUI/GUI.hpp"
#include "Hooks/Hooks.hpp"
#include "Serialization/Serialization.hpp"

#include "Interfaces.hpp"
#include "Memory.hpp"
#include "Netvars.hpp"

#include "Features/General.hpp"

// TODO Clantag
// TODO Force Crosshair
// TODO Radar

// TODO Glow & Chams

// TODO No Sleeves
// TODO No Scope Overlay
// TODO No Smoke
// TODO Flash Reducer

// TODO World & Sky Color

// TODO Reveal ranks, money, suspect, votes
// TODO Auto accept
// TODO Auto pistol

// TODO Moonwalk

// TODO Grenade Prediction & Brightness
// TODO Color Correction
// TODO Preserve Killfeed

// TODO Third person

void Initializer()
{
	srand(time(nullptr));

	struct stat info { };
	if (!(stat(Serialization::GetConfigDirectory(), &info) == 0 && S_ISDIR(info.st_mode)))
		mkdir(Serialization::GetConfigDirectory(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	Interfaces::GetInterfaces();

	Netvars::DumpNetvars();
	Memory::Create();

	Gui::Create();

	Hooks::InstallHooks();

	Features::General::EventLog::CreateReport(xorstr_("Initialized!"));
}

int __attribute__((constructor)) Startup()
{
	std::thread t(Initializer);
	t.detach();

	return 0;
}

void __attribute__((destructor)) Shutdown()
{
	Hooks::UninstallHooks();
	Gui::Destroy();
}
