#include "Memory.hpp"

#include <dlfcn.h>
#include <fcntl.h>
#include <link.h>
#include <sys/stat.h>
#include <vector>

#include "Interfaces.hpp"

#include "ldisasm.h"
#include "RetAddrSpoofer.hpp"
#include "xorstr.hpp"

#include "Utils/VMT.hpp"

#include "IDASignatureScanner.hpp"

void* RetAddrSpoofer::leaveRet = nullptr;

static void* lineGoesThroughSmoke;
static CBasePlayer** localPlayerList;

void* Memory::GetBaseAddress(const char* name)
{
	void* handle = dlmopen(LM_ID_BASE, name, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	if (!handle)
		return nullptr;
	link_map* linkMap;
	if (dlinfo(handle, RTLD_DL_LINKMAP, &linkMap) != 0) {
		dlclose(handle);
		return nullptr;
	}
	void* base = reinterpret_cast<void*>(linkMap->l_addr);
	dlclose(handle);
	return base;
}

void* Memory::RelativeToAbsolute(void* addr)
{
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return reinterpret_cast<char*>(addr) + 4 + *reinterpret_cast<int32_t*>(addr);
}

void Memory::Create()
{
	void* clientBaseAddress = GetBaseAddress(xorstr_("./csgo/bin/linux64/client_client.so"));

	void** baseClientVTable = Utils::GetVTable(Interfaces::baseClient);
	void** gameMovementVTable = Utils::GetVTable(Interfaces::gameMovement);

	// Set the address for the return address spoofer
	RetAddrSpoofer::leaveRet = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("c9 c3")), baseClientVTable[0]); // random code piece

	void* hudProcessInput = baseClientVTable[10];
	void* getClientMode = RelativeToAbsolute(reinterpret_cast<char*>(hudProcessInput) + 12);
	clientMode = *reinterpret_cast<void**>(RelativeToAbsolute(reinterpret_cast<char*>(getClientMode) + 4));

	void* hudUpdate = baseClientVTable[11];
	globalVars = *reinterpret_cast<CGlobalVars**>(RelativeToAbsolute(reinterpret_cast<char*>(hudUpdate) + 16));

	// void* in_activateMouse = baseClientVTable[16];
	// cinput = *reinterpret_cast<CInput**>(RelativeToAbsolute(reinterpret_cast<char*>(in_activateMouse) + 3));

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	void* categorizeGroundSurface = gameMovementVTable[69];

	void* leaInstr = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("48 8d 05") /* lea rax */), categorizeGroundSurface);
	moveHelper = *reinterpret_cast<IMoveHelper**>(RelativeToAbsolute(reinterpret_cast<char*>(leaInstr) + 3));

	// Has reference to "splitscreenplayer" inside itself
	void* fireGameEvent = Utils::GetVTable(clientMode)[55];
	void* movBeforeCall = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("48 89 85 10 d2 ff ff")), fireGameEvent);
	void* getLocalPlayer = Memory::RelativeToAbsolute(reinterpret_cast<char*>(movBeforeCall) + 8);

	localPlayerList = reinterpret_cast<CBasePlayer**>(RelativeToAbsolute(reinterpret_cast<char*>(getLocalPlayer) + 7));

	lineGoesThroughSmoke = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("55 48 89 e5 41 56 41 55 41 54 53 48 83 ec 30 8b 05 ?? ?? ?? ?? 66")), clientBaseAddress);
}

bool Memory::LineGoesThroughSmoke(const Vector& from, const Vector& to, const short _)
{
	// Little explanation why I make this struct here:
	// GCC for some reason decides that pushing the from and to Vector over general purpose registers is a good idea.
	// It basically creates pointers for these which are then pushed.
	// We want the XMM registers though, so it ends up completely destroying this function.
	// I create these VectorStructs in order to make GCC think, that XMM registers are the better choice.
	struct VectorStruct {
		float x, y, z;
	};

	VectorStruct fromStruct{};
	fromStruct.x = from.x;
	fromStruct.y = from.y;
	fromStruct.z = from.z;

	VectorStruct toStruct{};
	toStruct.x = to.x;
	toStruct.y = to.y;
	toStruct.z = to.z;

	return InvokeFunction<bool, VectorStruct, VectorStruct, short>(lineGoesThroughSmoke, fromStruct, toStruct, _);
}

CBasePlayer* Memory::GetLocalPlayer()
{
	return localPlayerList[0]; // There are no split screens in csgo, so safely assume we never want to have something else than 0
}
