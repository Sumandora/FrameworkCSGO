#include "Memory.hpp"

#include <dlfcn.h>
#include <fcntl.h>
#include <link.h>
#include <sys/stat.h>
#include <vector>

#include "Interfaces.hpp"

#include "xorstr.hpp"

#include "Utils/VMT.hpp"

#include "IDASignatureScanner.hpp"

void* RetAddrSpoofer::leaveRet = nullptr;

static void* lineGoesThroughSmoke;
static std::vector<dl_phdr_info> modules;

std::span<std::byte> Memory::GetTextSection(const char* name)
{
	for (const dl_phdr_info& module : modules) {
		if (std::strstr(module.dlpi_name, name)) {
			auto fd = open(module.dlpi_name, O_RDONLY); // Reopen it, since elf loaders can omit a lot of data

			struct stat st;
			if (fstat(fd, &st) != 0) {
				close(fd);
				continue;
			}

			void* base = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
			if (base == MAP_FAILED) {
				close(fd);
				continue;
			}

			const auto* elfHeader = reinterpret_cast<const Elf64_Ehdr*>(base);
			const auto* sectionHeaders = reinterpret_cast<const Elf64_Shdr*>(reinterpret_cast<const char*>(elfHeader) + elfHeader->e_shoff);
			const auto* stringSection = reinterpret_cast<const Elf64_Shdr*>(sectionHeaders + elfHeader->e_shstrndx);

			const char* stringTable = reinterpret_cast<const char*>(elfHeader) + stringSection->sh_offset;

			for (unsigned int i = 0; i < elfHeader->e_shnum; i++) {
				const auto* section = reinterpret_cast<const Elf64_Shdr*>(reinterpret_cast<std::uintptr_t>(sectionHeaders) + i * elfHeader->e_shentsize);
				if (section->sh_name != SHN_UNDEF) {
					const char* sectionName = stringTable + section->sh_name;

					if (std::strcmp(sectionName, xorstr_(".text")) == 0) {

						const std::span<std::byte> textSection{ reinterpret_cast<std::byte*>(module.dlpi_addr + section->sh_offset), static_cast<std::size_t>(section->sh_size) };

						munmap(base, st.st_size);
						close(fd);

						return textSection;
					}
				}
			}

			munmap(base, st.st_size);
			close(fd);

			return { /*rip*/ };
		}
	}
	// just crash here, doesn't matter
}

void* Memory::RelativeToAbsolute(void* addr)
{
	// RIP-Relatives start after the instruction using it
	// The relative offsets are 4 bytes long

	return static_cast<char*>(addr) + 4 + *static_cast<int32_t*>(addr);
}

void Memory::Create()
{
	dl_iterate_phdr([](dl_phdr_info* info, size_t, void* data) {
		auto* modules = reinterpret_cast<std::vector<dl_phdr_info>*>(data);
		modules->push_back(*info);
		return 0;
	},
		&modules);

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

	lineGoesThroughSmoke = SignatureScanner::FindNextOccurrence(SignatureScanner::BuildSignature(xorstr_("55 48 89 e5 41 56 41 55 41 54 53 48 83 ec 30 8b 05 ?? ?? ?? ?? 66")), GetTextSection(xorstr_("client_client.so")));
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