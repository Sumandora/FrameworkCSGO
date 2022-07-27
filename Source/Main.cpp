#include "Hooks/CreateMove/CreateMoveHook.hpp"
#include "Hooks/SDL/SDLHook.hpp"

#include "Interfaces.hpp"

#include <pthread.h>

void* Initializer(void*) {
	Interfaces::baseClient = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClient");
	Interfaces::engine = static_cast<CEngineClient*>(Interfaces::GetInterface("./bin/linux64/engine_client.so", "VEngineClient"));
	Interfaces::entityList = static_cast<CClientEntityList*>(Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClientEntityList"));

	Hooks::CreateMove::Hook();
	Hooks::SDL::Hook();
}

int __attribute((constructor)) Startup() {
	pthread_t thread;
	pthread_create(&thread, nullptr, Initializer, nullptr);

	return 0;
}
