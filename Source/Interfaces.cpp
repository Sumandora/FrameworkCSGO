#include "Interfaces.hpp"

#include <cstring>
#include <dlfcn.h>
#include <map>

#include "SDK/InterfaceReg.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "ldisasm.h"

#include "IDASignatureScanner.hpp"

void* Interfaces::UncoverCreateFunction(void* createFunc)
{
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
	Signature leaRax = SignatureScanner::BuildSignature(xorstr_("48 8d 05"));
	Signature movRaxRax = SignatureScanner::BuildSignature(xorstr_("48 8b 00"));
	Signature ret = SignatureScanner::BuildSignature(xorstr_("c3")); // Unnecessary but to follow the style

	char* rip = reinterpret_cast<char*>(createFunc);
	void* interfacePtr {};
	while (true) {
		if (SignatureScanner::DoesMatch(leaRax, rip)) { // LEA rax, [rip + offset]
			interfacePtr = Memory::RelativeToAbsolute(reinterpret_cast<char*>(rip) + 3 /* skip the lea */);
		} else if (SignatureScanner::DoesMatch(movRaxRax, rip)) { // MOV rax, [rax]
			interfacePtr = *reinterpret_cast<void**>(interfacePtr);
		} else if (SignatureScanner::DoesMatch(ret, rip)) { // RET
			break;
		}
		rip += ldisasm(rip, true); // next instruction
	}
	return interfacePtr;
}

template <typename T>
T* GetInterface(const char* file, const char* name)
{
	void* library = dlopen(file, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	if (!library)
		return nullptr;

	void* interfacesList = dlsym(library, xorstr_("s_pInterfaceRegs"));

	dlclose(library);

	if (!interfacesList)
		return nullptr;

	InterfaceReg* interface = *reinterpret_cast<InterfaceReg**>(interfacesList);

	while (interface != nullptr) {
		if (strncmp(interface->m_pName, name, strlen(interface->m_pName) - 3) == 0) {
			return reinterpret_cast<T*>(Interfaces::UncoverCreateFunction(interface->m_CreateFn));
		}
		interface = interface->m_pNext;
	}

	return nullptr;
}

void Interfaces::GetInterfaces()
{
	baseClient = GetInterface<void>(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClient"));
	engine = GetInterface<CEngineClient>(xorstr_("./bin/linux64/engine_client.so"), xorstr_("VEngineClient"));
	entityList = GetInterface<CClientEntityList>(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientEntityList"));
	engineTrace = GetInterface<CEngineTrace>(xorstr_("./bin/linux64/engine_client.so"), xorstr_("EngineTraceClient"));
	icvar = GetInterface<ICvar>(xorstr_("./bin/linux64/materialsystem_client.so"), xorstr_("VEngineCvar"));
	prediction = GetInterface<IPrediction>(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientPrediction"));
	gameMovement = GetInterface<CGameMovement>(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("GameMovement"));
}
