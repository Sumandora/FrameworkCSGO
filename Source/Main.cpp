#include <sys/stat.h>
#include <thread>

#include "xorstr.hpp"

#include "GUI/GUI.hpp"
#include "Hooks/Hooks.hpp"
#include "Serialization/Serialization.hpp"

#include "Interfaces.hpp"
#include "Memory.hpp"
#include "Netvars.hpp"

#include "Features/General/General.hpp"

// TODO Clantag
// TODO Force Crosshair
// TODO Radar

// TODO Glow & Chams

// TODO No Sleeves
// TODO No Scope Overlay
// TODO No Smoke

// TODO World & Sky Color

// TODO Reveal ranks, money, suspect (https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/client/c_playerresource.cpp#L426), votes
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
	if (!stat(Serialization::GetConfigDirectory(), &info) || !S_ISDIR(info.st_mode))
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
