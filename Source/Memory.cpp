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
	
	// This method can be used to simulate recoil
	// https://github.com/SwagSoftware/Kisak-Strike/blob/7df358a4599ba02a4e072f8167a65007c9d8d89c/game/shared/cstrike15/weapon_csbase.cpp#L3838
	csweaponbase_recoil =
		Pattern(xorstr_(
			"\x55"				// push %rbp
			"\x48\x89\xe5"		// mov %rsp,%rsp
			"\x41\x57"			// push %r15
			"\x41\x56"			// push %r14
			"\x41\x55"			// push %r13
			"\x41\x89\xf5"		// mov %esi,%r13d
			"\x41\x54"			// push %r12
			"\x53"				// push %rbx
			"\x48\x89\xfb"		// mov %rdi,%rbx
			"\x48\x83\xec\x18"	// sub $0x18,%rsp ; Because we are ensuring the method prolog here, we can hardcode this number, because it is depending on the prolog
			"\xe8\00\00\00\00"	// call fun_XXXXXXXX ; This will change
			"\x48\x85\xc0"		// test %rax,%rax
			), xorstr_("xxxxxxxxxxxxxxxxxxxxxxxx????xxx")
		).searchPattern(
			GetBaseAddress(xorstr_("./csgo/bin/linux64/client_client.so"))
		);
}
