#include <thread>

#include "GUI/GUI.hpp"
#include "Hooks/Hooks.hpp"

#include "Interfaces.hpp"
#include "Memory.hpp"
#include "Netvars.hpp"
#include "xorstr.hpp"

void Initializer() {
	Interfaces::baseClient	= Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClient"));
	Interfaces::engine		= reinterpret_cast<CEngineClient*>(Interfaces::GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("VEngineClient")));
	Interfaces::entityList	= reinterpret_cast<CClientEntityList*>(Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientEntityList")));
	Interfaces::engineTrace = reinterpret_cast<CEngineTrace*>(Interfaces::GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("EngineTraceClient")));

	Netvars::DumpNetvars();
	Memory::Create();

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
