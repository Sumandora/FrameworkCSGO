#include "Interfaces.hpp"
#include <cstring>
#include <dlfcn.h>

struct InterfaceReg
{
	void* m_CreateFn;
	const char *m_pName;
	InterfaceReg *m_pNext;
};

void* Interfaces::GetInterface(const char* file, const char* name) {
	void* library = dlopen(file, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	if(!library) return nullptr;

	void* interfacesList = dlsym(library, "s_pInterfaceRegs");
	
	dlclose(library);
	
	if(!interfacesList) return nullptr;

	InterfaceReg* interface = *reinterpret_cast<InterfaceReg**>(interfacesList);

	while((interface = interface->m_pNext) != nullptr) {
		if(strncmp(interface->m_pName, name, strlen(interface->m_pName) - 3) == 0) {
			/**
			 * These functions look like this:
			 * push rbp ; Stack frame - we dont care
			 * lea rax, [rip + address] ; We want this address
			 * yadda yadda yadda
			 */
			
			char* relAddr = static_cast<char*>(interface->m_CreateFn) + 4 /* Offset to skip push and lea opcodes (push = 1; lea = 3 + address) */;
			// Address + Length of Address + relative Offset -> Absolute Address
			return relAddr + 4 + *reinterpret_cast<int*>(relAddr);
		}
	}

	return nullptr;
}
