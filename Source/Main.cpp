#include <sys/stat.h>
#include <thread>

#include "GUI/GUI.hpp"
#include "Hooks/Hooks.hpp"
#include "Serialization/Serialization.hpp"

#include "ConVarStorage.hpp"
#include "Interfaces.hpp"
#include "Memory.hpp"
#include "Netvars.hpp"

#include "xorstr.hpp"

void Initializer()
{
	srand(time(nullptr));

	struct stat info;
	if (!(stat(Serialization::GetConfigDirectory(), &info) == 0 && S_ISDIR(info.st_mode)))
		mkdir(Serialization::GetConfigDirectory(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	Interfaces::GetInterfaces();

	Netvars::DumpNetvars();
	Memory::Create();

	ConVarStorage::FindAll();

	Gui::Create();

	Hooks::InstallHooks();
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
