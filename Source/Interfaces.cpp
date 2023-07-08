#include "Interfaces.hpp"

#include <cassert>
#include <cstring>
#include <dlfcn.h>
#include <map>

#include "SDK/InterfaceReg.hpp"

#include "imgui.h"

#include "BCRL.hpp"

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

	char* rip = reinterpret_cast<char*>(createFunc);
	void* interfacePtr = nullptr;
	BCRL::Session::Pointer(rip)
		.Repeater([&interfacePtr](BCRL::SafePointer& pointer) {
			if (pointer.DoesMatch("48 8d 05")) { // LEA rax, [rip + offset]
				interfacePtr = pointer.Add(3).RelativeToAbsolute().GetPointer();
			} else if (pointer.DoesMatch("48 8b 00")) { // MOV rax, [rax]
				interfacePtr = *reinterpret_cast<void**>(interfacePtr);
			} else if (pointer.DoesMatch("c3")) { // RET
				return false;
			}

			pointer = pointer.NextInstruction();
			return true;
		});
	return interfacePtr;
}

template <typename T>
T* GetInterface(void* handle, const char* name)
{
	void* interfacesList = dlsym(handle, "s_pInterfaceRegs");

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
	void* client_client = dlmopen(LM_ID_BASE, "./csgo/bin/linux64/client_client.so", RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	void* engine_client = dlmopen(LM_ID_BASE, "./bin/linux64/engine_client.so", RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	void* materialsystem_client = dlmopen(LM_ID_BASE, "./bin/linux64/materialsystem_client.so", RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);

	baseClient = GetInterface<void>(client_client, "VClient");
	engine = GetInterface<CEngineClient>(engine_client, "VEngineClient");
	entityList = GetInterface<CClientEntityList>(client_client, "VClientEntityList");
	engineTrace = GetInterface<CEngineTrace>(engine_client, "EngineTraceClient");
	icvar = GetInterface<ICvar>(materialsystem_client, "VEngineCvar");
	prediction = GetInterface<IPrediction>(client_client, "VClientPrediction");
	gameMovement = GetInterface<CGameMovement>(client_client, "GameMovement");
	materialSystem = GetInterface<CMaterialSystem>(materialsystem_client, "VMaterialSystem");
	engineRenderView = GetInterface<void>(engine_client, "VEngineRenderView");

	dlclose(client_client);
	dlclose(engine_client);
	dlclose(materialsystem_client);
}
