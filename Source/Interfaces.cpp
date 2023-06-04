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
	const Signature leaRax = SignatureScanner::BuildSignature(xorstr_("48 8d 05"));
	const Signature movRaxRax = SignatureScanner::BuildSignature(xorstr_("48 8b 00"));
	const Signature ret = SignatureScanner::BuildSignature(xorstr_("c3")); // Unnecessary but to follow the style

	char* rip = reinterpret_cast<char*>(createFunc);
	void* interfacePtr{};
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
T* GetInterface(void* handle, const char* name)
{
	void* interfacesList = dlsym(handle, xorstr_("s_pInterfaceRegs"));

	if (!interfacesList)
		return nullptr;

	;

	for (InterfaceReg* interface = *reinterpret_cast<InterfaceReg**>(interfacesList); interface; interface = interface->m_pNext) {
		if (std::strncmp(interface->m_pName, name, std::strlen(interface->m_pName) - 3) == 0) {
			return reinterpret_cast<T*>(Interfaces::UncoverCreateFunction(interface->m_CreateFn));
		}
	}

	return nullptr;
}

void Interfaces::GetInterfaces()
{
	void* client_client = dlopen(xorstr_("./csgo/bin/linux64/client_client.so"), RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	void* engine_client = dlopen(xorstr_("./bin/linux64/engine_client.so"), RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	void* materialsystem_client = dlopen(xorstr_("./bin/linux64/materialsystem_client.so"), RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);

	baseClient = GetInterface<void>(client_client, xorstr_("VClient"));
	engine = GetInterface<CEngineClient>(engine_client, xorstr_("VEngineClient"));
	entityList = GetInterface<CClientEntityList>(client_client, xorstr_("VClientEntityList"));
	engineTrace = GetInterface<CEngineTrace>(engine_client, xorstr_("EngineTraceClient"));
	icvar = GetInterface<ICvar>(materialsystem_client, xorstr_("VEngineCvar"));
	prediction = GetInterface<IPrediction>(client_client, xorstr_("VClientPrediction"));
	gameMovement = GetInterface<CGameMovement>(client_client, xorstr_("GameMovement"));
	materialSystem = GetInterface<CMaterialSystem>(materialsystem_client, xorstr_("VMaterialSystem"));
	engineRenderView = GetInterface<void>(engine_client, xorstr_("VEngineRenderView"));

	dlclose(client_client);
	dlclose(engine_client);
	dlclose(materialsystem_client);
}
