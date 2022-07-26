#include "Interfaces.hpp"
#include "Hooking/Hooking.hpp"
#include "ReturnAddr/ReturnAddr.hpp"
#include "PatternScan/PatternScan.hpp"

#include <cstdio>

#define FL_ONGROUND	(1<<0)
#define IN_JUMP	(1 << 1)

struct Vector {
	float x, y, z;
};

struct CUserCmd
{
	virtual ~CUserCmd() {};
	int command_number;
	int tick_count;
	Vector viewangles;
	Vector aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	unsigned char impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
	Vector headangles;
	Vector headoffset;
};

void* entityList;
void* engine;

void* proxy;
bool __attribute((optimize("O0"),noinline)) CreateMove_Hook(void* thisptr, float flInputSampleTime, CUserCmd* cmd) {
	if(!Framework::ReturnAddr::ret_instruction_addr)
		Framework::ReturnAddr::ret_instruction_addr =
			Framework::ReturnAddr::leave_ret_instruction.searchPattern(
				__builtin_extract_return_addr(__builtin_return_address(0))
			);

	bool silent = Framework::ReturnAddr::invoke<bool, void*, float, CUserCmd*>(proxy, thisptr, flInputSampleTime, cmd);

	if(!cmd || !cmd->command_number)
		return silent;

	int localPlayerIndex = Framework::ReturnAddr::invoke<int, void*>((*(void***)engine)[12], engine);
	uintptr_t localPlayer = Framework::ReturnAddr::invoke<uintptr_t, void*, int>((*(void***)entityList)[3], entityList, localPlayerIndex);

	if(!localPlayer)
		return silent;

	printf("Player: %d/%ld\n", localPlayerIndex, localPlayer);
	int flags = *(int*)(localPlayer + 0x13c);
	printf("Flags: %d\n", flags);
	printf("Ground: %d\n", flags & FL_ONGROUND);
	
	if(cmd->buttons & IN_JUMP) {
		if(!(flags & FL_ONGROUND)) {
			printf("Off Ground\n");
			cmd->buttons &= ~IN_JUMP;
		}
	}

	return silent || true;
}

int __attribute((constructor)) Startup() {
	void* baseClient = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClient");
	engine = Interfaces::GetInterface("./bin/linux64/engine_client.so", "VEngineClient");
	entityList = Interfaces::GetInterface("./csgo/bin/linux64/client_client.so", "VClientEntityList");
	
	void* hudProcessInput = (*(void***)baseClient)[10];

	
	/**
	 * This method calls GetClientMode()
	 * Call instruction is at the 11th byte
	 * The next byte is the address
	 */
	char* relAddr = static_cast<char*>(hudProcessInput) + 12;
	void* getClientMode = relAddr + 4 + *reinterpret_cast<int*>(relAddr);
	
	relAddr = static_cast<char*>(getClientMode) + 4; // Skip push and mov opcodes
	void* ClientMode = *(void**) (relAddr + 4 + *reinterpret_cast<int*>(relAddr));
	
	void* CreateMove = (*(void***)ClientMode)[25];

	proxy = Framework::Hooking::detour(CreateMove, (void*) CreateMove_Hook, 6);
	
	return 0;
}
