#include <thread>

#include "Hooks/CreateMove/CreateMoveHook.hpp"
#include "Hooks/SDL/SDLHook.hpp"
#include "Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include "GUI/GUI.hpp"

#include "Interfaces.hpp"
#include "Netvars.hpp"
#include "Memory.hpp"
#include "xorstr.hpp"

class CClientEntityList;
class CEngineClient;
class CEngineTrace;

void Initializer() {
	Interfaces::baseClient	= Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClient"));
	Interfaces::engine		= static_cast<CEngineClient*>(Interfaces::GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("VEngineClient")));
	Interfaces::entityList	= static_cast<CClientEntityList*>(Interfaces::GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientEntityList")));
	Interfaces::engineTrace	= static_cast<CEngineTrace*>(Interfaces::GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("EngineTraceClient")));
	
	Netvars::DumpNetvars();
	Memory::Create();
	
	Gui::Create();
	
	Hooks::FrameStageNotify::Hook();
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
	Hooks::FrameStageNotify::Unhook();

	Gui::Destroy();
}
