#include "Memory.hpp"

#include "Interfaces.hpp"
#include "xorstr.hpp"

#include "PatternScan/PatternScan.hpp"

#include "Utils/VMT.hpp"

void* Memory::RelativeToAbsolute(void* addr)
{
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return static_cast<char*>(addr) + 4 + *reinterpret_cast<int*>(addr);
}

void Memory::Create()
{
	void** baseClientVTable = Utils::GetVTable(Interfaces::baseClient);

	// Set the address for the return address spoofer
	ret_instruction_addr = Pattern(
		xorstr_("\xC9\xC3"), // leave; ret; instructions
		xorstr_("xx"))
							   .searchPattern(baseClientVTable[0]); // random code piece

	void* hudProcessInput = baseClientVTable[10];
	void* hudUpdate = baseClientVTable[11];

	void* getClientMode = RelativeToAbsolute(static_cast<char*>(hudProcessInput) + 12);

	clientMode = *reinterpret_cast<void**>(RelativeToAbsolute(static_cast<char*>(getClientMode) + 4));
	globalVars = *reinterpret_cast<CGlobalVars**>(RelativeToAbsolute(static_cast<char*>(hudUpdate) + 16));

	void** gameMovementVTable = Utils::GetVTable(Interfaces::gameMovement);

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	void* categorizeGroundSurface = gameMovementVTable[69];

	void* leaInstr = Pattern("\x48\x8d\x05" /* lea rax */, "xxx").searchPattern(categorizeGroundSurface);
	moveHelper = *reinterpret_cast<IMoveHelper**>(RelativeToAbsolute(reinterpret_cast<char*>(leaInstr) + 3));
}
