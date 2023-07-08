#include "../Debug.hpp"

#include <link.h>
#include <mutex>

#include "../../../../Interfaces.hpp"
#include "../../../../SDK/InterfaceReg.hpp"
#include "../../../Elements/HelpMarker.hpp"

void Gui::Windows::Interfaces()
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
			void* library = dlmopen(LM_ID_BASE, info->dlpi_name, RTLD_NOW | RTLD_NOLOAD | RTLD_LOCAL);
			if (!library)
				return 0;

			void* interfaces = dlsym(library, "s_pInterfaceRegs");
			if (!interfaces) {
				dlclose(library);
				return 0;
			}

			interfaceStorage[info->dlpi_name] = {};

			InterfaceReg* interface = *static_cast<InterfaceReg**>(interfaces);

			while (interface != nullptr) {
				Interface t{};
				t.reg = interface;
				t.uncoveredAddress = nullptr;
				t.realAddress = nullptr;
				interfaceStorage[info->dlpi_name][interface->m_pName] = t;
				interface = interface->m_pNext;
			}
			dlclose(library);
			return 0;
		},
			nullptr);
	});

	for (const auto& [sharedObject, interfaces] : interfaceStorage) {
		if (ImGui::TreeNode(sharedObject)) {
			for (const auto& [interfaceName, cachedInterface] : interfaces) {
				if (ImGui::TreeNode(interfaceName)) {
					ImGui::Text("Create function: %p", cachedInterface.reg->m_CreateFn);
					ImGui::Text("Name: %s", cachedInterface.reg->m_pName);
					ImGui::Text("Next: %p", cachedInterface.reg->m_pNext);

					ImGui::Separator();

					ImGui::Text("The next two addresses should line up, if they don't, we have to adjust the uncover function.");

					if (cachedInterface.uncoveredAddress) {
						ImGui::Text("Uncovered address: %p", cachedInterface.uncoveredAddress);
					} else {
						if (ImGui::Button("Uncover create function")) {
							cachedInterface.uncoveredAddress = Interfaces::UncoverCreateFunction(
								cachedInterface.reg->m_CreateFn);
						}
					}

					if (cachedInterface.realAddress) {
						ImGui::Text("Real address: %p", cachedInterface.realAddress);
					} else {
						if (ImGui::Button("Invoke create function")) {
							cachedInterface.realAddress = ((void* (*)())(cachedInterface.reg->m_CreateFn))();
						}
						ImGui::HelpMarker("Warning: This is considered unsafe");
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}
