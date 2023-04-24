#include "Memory.hpp"

#include <dlfcn.h>
#include <vector>

#include "Interfaces.hpp"
#include "xorstr.hpp"

#include "Utils/VMT.hpp"

#include "IDASignatureScanner.hpp"

void* RetAddrSpoofer::leaveRet = nullptr;

static void* lineGoesThroughSmoke;

void* GetBaseAddress(const char* name)
{
	void* handle = dlopen(name, RTLD_NOLOAD | RTLD_NOW);
	void* baseAddress = *static_cast<void**>(handle);
	dlclose(handle); // Reset ref count
	return baseAddress;
}

void* Memory::RelativeToAbsolute(void* addr)
{
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return static_cast<char*>(addr) + 4 + *static_cast<int32_t*>(addr);
}

void Memory::Create()
{
	void** baseClientVTable = Utils::GetVTable(Interfaces::baseClient);
	void** gameMovementVTable = Utils::GetVTable(Interfaces::gameMovement);

	// Set the address for the return address spoofer
	RetAddrSpoofer::leaveRet = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("c9 c3")), baseClientVTable[0]); // random code piece

	void* hudProcessInput = baseClientVTable[10];
	void* hudUpdate = baseClientVTable[11];

	void* getClientMode = RelativeToAbsolute(static_cast<char*>(hudProcessInput) + 12);

	clientMode = *static_cast<void**>(RelativeToAbsolute(static_cast<char*>(getClientMode) + 4));
	globalVars = *static_cast<CGlobalVars**>(RelativeToAbsolute(static_cast<char*>(hudUpdate) + 16));

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	void* categorizeGroundSurface = gameMovementVTable[69];

	void* leaInstr = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("48 8d 05") /* lea rax */), categorizeGroundSurface);
	moveHelper = *static_cast<IMoveHelper**>(RelativeToAbsolute(reinterpret_cast<char*>(leaInstr) + 3));

	lineGoesThroughSmoke = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("55 48 89 e5 41 56 41 55 41 54 53 48 83 ec 30 8b 05 ?? ?? ?? ?? 66")), GetBaseAddress(xorstr_("./csgo/bin/linux64/client_client.so")));
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

	VectorStruct fromStruct {};
	fromStruct.x = from.x;
	fromStruct.y = from.y;
	fromStruct.z = from.z;

	VectorStruct toStruct {};
	toStruct.x = to.x;
	toStruct.y = to.y;
	toStruct.z = to.z;

	return InvokeFunction<bool, VectorStruct, VectorStruct, short>(lineGoesThroughSmoke, fromStruct, toStruct, _);
}