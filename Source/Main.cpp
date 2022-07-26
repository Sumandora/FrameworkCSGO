#include "ReturnAddr/ReturnAddr.hpp"
#include "Hooking/Hooking.hpp"
#include "PatternScan/PatternScan.hpp"

#include "Interfaces.hpp"
#include "Hooks/CreateMove/CreateMoveHook.hpp"

#include <cstdio>

int __attribute((constructor)) Startup() {
	printf("Loaded library\n");
	Interfaces::baseClient = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClient");
	Interfaces::engine = Interfaces::GetInterface("./bin/linux64/engine_client.so", "VEngineClient");
	Interfaces::entityList = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClientEntityList");
	
	void* hudProcessInput = (*(void***)Interfaces::baseClient)[10];

	
	/**
	 * This method calls GetClientMode()
	 * Call instruction is at the 11th byte
	 * The next byte is the address
	 */
	char* relAddr = static_cast<char*>(hudProcessInput) + 12;
	void* getClientMode = relAddr + 4 + *reinterpret_cast<int*>(relAddr);
	
	relAddr = static_cast<char*>(getClientMode) + 4; // Skip push and mov opcodes
	void* clientMode = *(void**) (relAddr + 4 + *reinterpret_cast<int*>(relAddr));
	
	void* createMove = (*(void***)clientMode)[25];

	Hooks::CreateMove::proxy = Framework::Hooking::detour(createMove, (void*) Hooks::CreateMove::CreateMoveHook, 6);
	printf("CreateMove has been detour'd");

	return 0;
}
