#include "Memory.hpp"

#include <dlfcn.h>
#include <link.h>
#include <vector>

#include "Interfaces.hpp"
#include "xorstr.hpp"

#include "PatternScan/PatternScan.hpp"

#include "Utils/VMT.hpp"
struct dlinfo_t {
	const char* library = nullptr;
	uintptr_t address = 0;
	size_t size = 0;
};

std::vector<dlinfo_t> libraries;

// taken form aixxe's cstrike-basehook-linux
bool GetLibraryInformation(const char* library, uintptr_t* address, size_t* size)
{
	if (libraries.size() == 0) {
		dl_iterate_phdr([](struct dl_phdr_info* info, size_t, void*) {
			dlinfo_t library_info = {};

			library_info.library = info->dlpi_name;
			library_info.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
			library_info.size = info->dlpi_phdr[0].p_memsz;

			libraries.push_back(library_info);

			return 0;
		},
			nullptr);
	}

	for (const dlinfo_t& current : libraries) {
		if (!strcasestr(current.library, library))
			continue;

		if (address)
			*address = current.address;

		if (size)
			*size = current.size;

		return true;
	}

	return false;
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
		xorstr_("\xC9\xC3"), // leave; ret; instructions
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

	void* leaInstr = Pattern(xorstr_("\x48\x8d\x05") /* lea rax */, xorstr_("xxx")).searchPattern(categorizeGroundSurface);
	moveHelper = *reinterpret_cast<IMoveHelper**>(RelativeToAbsolute(reinterpret_cast<char*>(leaInstr) + 3));
}
