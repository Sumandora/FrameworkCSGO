#include "Interfaces.hpp"
#include <cstring>
#include <dlfcn.h>

#include "xorstr.hpp"

struct InterfaceReg {
	void*		  m_CreateFn;
	const char*	  m_pName;
	InterfaceReg* m_pNext;
};

void* Interfaces::GetInterface(const char* file, const char* name) {
	void* library = dlopen(file, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	if (!library)
		return nullptr;

	void* interfacesList = dlsym(library, xorstr_("s_pInterfaceRegs"));

	dlclose(library);

	if (!interfacesList)
		return nullptr;

	InterfaceReg* interface = *reinterpret_cast<InterfaceReg**>(interfacesList);

	while ((interface = interface->m_pNext) != nullptr) {
		if (strncmp(interface->m_pName, name, strlen(interface->m_pName) - 3) == 0) {
			/**
			 * These functions look like this:
			 * push rbp ; Stack frame - we don't care
			 * lea rax, [rip + address] ; We want this address
			 * yadda yadda yadda
			 * 
			 * Heyyy, me from the future here.
			 * Man life changed, blah blah.
			 * Basically the function looks different sometimes...
			 * We are walking through the function and emulate all instructions we need.
			 * 
			 * Just in case someone wonders why I was sure back then, that the instructions would never change is the following:
			 * There is a preprocessor definition in the source engine which creates these exported interface functions.
			 * But hey who could imagine that the same code also produces the same assembly right?
			 */
			static Pattern leaRax = Pattern("\x48\x8d\x05", "xxx");
			static Pattern movRaxRax = Pattern("\x48\x8b\x00", "xxx");
			static Pattern ret = Pattern("\xc3", "x"); // Unnecessary but to follow the style
			
			char* rip = reinterpret_cast<char*>(interface->m_CreateFn);
			void* interfacePtr;
			while(true) {
				if(leaRax.matchPattern(rip)) { // LEA rax, [rip + offset]
					interfacePtr = Memory::RelativeToAbsolute(reinterpret_cast<char*>(rip) + 3 /* skip the lea */);
					rip += 7;
				} else if(movRaxRax.matchPattern(rip)) { // MOV rax, [rax]
					interfacePtr = *reinterpret_cast<void**>(interfacePtr);
					rip += 3;
				} else if(ret.matchPattern(rip)) { // RET
					break;
				}
				rip++;
			}
			return interfacePtr;
		}
	}

	return nullptr;
}
