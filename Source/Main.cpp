#include "Hooks/CreateMove/CreateMoveHook.hpp"
#include "Hooks/SDL/SDLHook.hpp"

#include "Interfaces.hpp"
#include "Netvars.hpp"
#include "SDK/GameClass/VirtualMethod.hpp"
#include "Memory.hpp"
#include "GUI.hpp"

#include "xorstr.hpp"

#include <thread>

void Initializer() {
	Interfaces::baseClient = Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClient"));
	Interfaces::engine = static_cast<CEngineClient*>(Interfaces::GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("VEngineClient")));
	Interfaces::entityList = static_cast<CClientEntityList*>(Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientEntityList")));
	
	Netvars::DumpNetvars();
	Memory::Create();
	
	Gui::Create();
	
	Hooks::CreateMove::Hook();
	Hooks::SDL::Hook();
}

int __attribute__((constructor)) Startup() {
	std::thread t(Initializer);

	t.detach();

	return 0;
}

void __attribute__((destructor)) Shutdown() {
	Hooks::SDL::Unhook();
	Hooks::CreateMove::Unhook();

	Gui::Destroy();
}
