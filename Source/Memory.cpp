#include "Memory.hpp"

#include "ReturnAddr/ReturnAddr.hpp"
#include "Utils/VMT.hpp"
#include "Interfaces.hpp"

#include <dlfcn.h>

void* GetBaseAddress(const char* library) {
	void* handle = dlopen(library, RTLD_NOW | RTLD_NOLOAD);
	void* addr = *reinterpret_cast<void**>(handle);
	dlclose(handle);
	return addr;
}

void* relToAbsAddress(void* addr) {
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return static_cast<char*>(addr) + 4 + *reinterpret_cast<int*>(addr);
}

void Memory::Create() {
	// Set the address for the return address spoofer
	ret_instruction_addr =
		Framework::ReturnAddr::leave_ret_instruction.searchPattern(
			Utils::GetVTable(Interfaces::baseClient)[0] // random code piece
		);

	void** baseClientVTable = Utils::GetVTable(Interfaces::baseClient);
	
	void* hudProcessInput	= baseClientVTable[10];
	void* hudUpdate			= baseClientVTable[11];
	
	void* getClientMode		= relToAbsAddress(static_cast<char*>(hudProcessInput) + 12);
	
	Interfaces::clientMode	= *reinterpret_cast<void**>(relToAbsAddress(static_cast<char*>(getClientMode) + 4));
	Interfaces::globalVars	= *reinterpret_cast<CGlobalVars**>(relToAbsAddress(static_cast<char*>(hudUpdate) + 16));
}
