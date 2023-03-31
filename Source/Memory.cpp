#include "Memory.hpp"

#include <dlfcn.h>
#include <vector>

#include "Interfaces.hpp"
#include "xorstr.hpp"

#include "PatternScan/PatternScan.hpp"

#include "Utils/VMT.hpp"

static void* lineGoesThroughSmoke;

void* GetBaseAddress(const char* name)
{
	void* handle = dlopen(name, RTLD_NOLOAD | RTLD_NOW);
	void* baseAddress = *reinterpret_cast<void**>(handle);
	dlclose(handle); // Reset ref count
	return baseAddress;
}

void* Memory::RelativeToAbsolute(void* addr)
{
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return static_cast<char*>(addr) + 4 + *reinterpret_cast<int*>(addr);
}

void Memory::Create()
{
	void** baseClientVTable = Utils::GetVTable(Interfaces::baseClient);
	void** gameMovementVTable = Utils::GetVTable(Interfaces::gameMovement);

	// Set the address for the return address spoofer
	ret_instruction_addr = Pattern(
		"\xC9\xC3", // leave; ret; instructions
		xorstr_("xx"))
							   .searchPattern(baseClientVTable[0]); // random code piece

	void* hudProcessInput = baseClientVTable[10];
	void* hudUpdate = baseClientVTable[11];

	void* getClientMode = RelativeToAbsolute(static_cast<char*>(hudProcessInput) + 12);

	clientMode = *reinterpret_cast<void**>(RelativeToAbsolute(static_cast<char*>(getClientMode) + 4));
	globalVars = *reinterpret_cast<CGlobalVars**>(RelativeToAbsolute(static_cast<char*>(hudUpdate) + 16));

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	void* categorizeGroundSurface = gameMovementVTable[69];

	void* leaInstr = Pattern("\x48\x8d\x05" /* lea rax */, xorstr_("xxx")).searchPattern(categorizeGroundSurface);
	moveHelper = *reinterpret_cast<IMoveHelper**>(RelativeToAbsolute(reinterpret_cast<char*>(leaInstr) + 3));

	lineGoesThroughSmoke = Pattern("\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x54\x53\x48\x83\xEC\x30\x8B\x05\x00\x00\x00\x00\x66", xorstr_("xxxxxxxxxxxxxxxxx????x"))
							   .searchPattern(GetBaseAddress(xorstr_("./csgo/bin/linux64/client_client.so")));
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

	return invokeFunction<bool, VectorStruct, VectorStruct, short>(lineGoesThroughSmoke, fromStruct, toStruct, _);
}