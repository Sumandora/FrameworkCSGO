#include "Interfaces.hpp"

#include "PatternScan/PatternScan.hpp"

#include <cstring>
#include <dlfcn.h>
#include <link.h>
#include <map>
#include <mutex>

#include "GUI/Elements/HelpMarker.hpp"
#include "imgui.h"
#include "xorstr.hpp"

struct InterfaceReg {
	void* m_CreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;
};

void* UncoverCreateFunction(void* createFunc)
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
	Pattern leaRax = Pattern(xorstr_("\x48\x8d\x05"), xorstr_("xxx"));
	Pattern movRaxRax = Pattern("\x48\x8b\x00", xorstr_("xxx"));
	Pattern ret = Pattern("\xc3", "x"); // Unnecessary but to follow the style

	char* rip = static_cast<char*>(createFunc);
	void* interfacePtr {};
	while (true) {
		if (leaRax.matchPattern(rip)) { // LEA rax, [rip + offset]
			interfacePtr = Memory::RelativeToAbsolute(static_cast<char*>(rip) + 3 /* skip the lea */);
			rip += 7;
		} else if (movRaxRax.matchPattern(rip)) { // MOV rax, [rax]
			interfacePtr = *static_cast<void**>(interfacePtr);
			rip += 3;
		} else if (ret.matchPattern(rip)) { // RET
			break;
		}
		rip++;
	}
	return interfacePtr;
}

void* GetInterface(const char* file, const char* name)
{
	void* library = dlopen(file, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
	if (!library)
		return nullptr;

	void* interfacesList = dlsym(library, xorstr_("s_pInterfaceRegs"));

	dlclose(library);

	if (!interfacesList)
		return nullptr;

	InterfaceReg* interface = *static_cast<InterfaceReg**>(interfacesList);

	while (interface != nullptr) {
		if (strncmp(interface->m_pName, name, strlen(interface->m_pName) - 3) == 0) {
			return UncoverCreateFunction(interface->m_CreateFn);
		}
		interface = interface->m_pNext;
	}

	return nullptr;
}

void Interfaces::GetInterfaces()
{
	baseClient = GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClient"));
	engine = static_cast<CEngineClient*>(GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("VEngineClient")));
	entityList = static_cast<CClientEntityList*>(GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientEntityList")));
	engineTrace = static_cast<CEngineTrace*>(GetInterface(xorstr_("./bin/linux64/engine_client.so"), xorstr_("EngineTraceClient")));
	icvar = static_cast<ICvar*>(GetInterface(xorstr_("./bin/linux64/materialsystem_client.so"), xorstr_("VEngineCvar")));
	prediction = static_cast<IPrediction*>(GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("VClientPrediction")));
	gameMovement = static_cast<CGameMovement*>(GetInterface(xorstr_("./csgo/bin/linux64/client_client.so"), xorstr_("GameMovement")));
}

void Interfaces::SetupGUI()
{
	struct Interface {
		InterfaceReg* reg;
		mutable void* uncoveredAddress;
		mutable void* realAddress;
	};

	static std::map<const char*, std::map<const char*, Interface>> interfaceStorage;
	static std::once_flag init;

	std::call_once(init, []() {
		dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
			void* library = dlopen(info->dlpi_name, RTLD_NOLOAD | RTLD_NOW);
			if (!library)
				return 0;

			void* interfaces = dlsym(library, xorstr_("s_pInterfaceRegs"));
			dlclose(library);
			if (!interfaces)
				return 0;

			interfaceStorage[info->dlpi_name] = {};

			InterfaceReg* interface = *static_cast<InterfaceReg**>(interfaces);

			while (interface != nullptr) {
				Interface t {};
				t.reg = interface;
				t.uncoveredAddress = nullptr;
				t.realAddress = nullptr;
				interfaceStorage[info->dlpi_name][interface->m_pName] = t;
				interface = interface->m_pNext;
			}
			return 0;
		},
			nullptr);
	});

	for (const auto& [sharedObject, interfaces] : interfaceStorage) {
		if (ImGui::TreeNode(sharedObject)) {
			for (const auto& [interfaceName, cachedInterface] : interfaces) {
				if (ImGui::TreeNode(interfaceName)) {
					ImGui::Text(xorstr_("Create function: %p"), cachedInterface.reg->m_CreateFn);
					ImGui::Text(xorstr_("Name: %s"), cachedInterface.reg->m_pName);
					ImGui::Text(xorstr_("Next: %p"), cachedInterface.reg->m_pNext);

					ImGui::Separator();

					ImGui::Text(xorstr_("The next two addresses should line up, if they don't, we have to adjust the uncover function."));

					if (cachedInterface.uncoveredAddress) {
						ImGui::Text(xorstr_("Uncovered address: %p"), cachedInterface.uncoveredAddress);
					} else {
						if (ImGui::Button(xorstr_("Uncover create function"))) {
							cachedInterface.uncoveredAddress = UncoverCreateFunction(cachedInterface.reg->m_CreateFn);
						}
					}

					if (cachedInterface.realAddress) {
						ImGui::Text(xorstr_("Real address: %p"), cachedInterface.realAddress);
					} else {
						if (ImGui::Button(xorstr_("Invoke create function"))) {
							cachedInterface.realAddress = ((void*(*)())(cachedInterface.reg->m_CreateFn))();
						}
						ImGui::HelpMarker(xorstr_("Warning: This is considered unsafe"));
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}
