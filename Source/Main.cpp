#include "Hooks/CreateMove/CreateMoveHook.hpp"
#include "Interfaces.hpp"

#include <cstdio>

int __attribute((constructor)) Startup() {
	printf("Loaded library\n");
	Interfaces::baseClient = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClient");
	Interfaces::engine = static_cast<CEngineClient*>(Interfaces::GetInterface("./bin/linux64/engine_client.so", "VEngineClient"));
	Interfaces::entityList = static_cast<CClientEntityList*>(Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClientEntityList"));

	Hooks::CreateMove::Hook();
	
	return 0;
}
