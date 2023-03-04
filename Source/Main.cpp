#include <thread>

#include "GUI/GUI.hpp"
#include "Hooks/Hooks.hpp"

#include "Interfaces.hpp"
#include "Memory.hpp"
#include "Netvars.hpp"
#include "ConVarStorage.hpp"

#include "xorstr.hpp"

void Initializer() {
	srand(time(nullptr));

	Interfaces::GetInterfaces();

	Netvars::DumpNetvars();
	Memory::Create();

	ConVarStorage::FindAll();

	Gui::Create();

	Hooks::InstallHooks();
}

int __attribute__((constructor)) Startup() {
	std::thread t(Initializer);
	t.detach();

	return 0;
}

void __attribute__((destructor)) Shutdown() {
	Hooks::UninstallHooks();
	Gui::Destroy();
}
