#include "../Debug.hpp"

#include "BCRL.hpp"
#include "imgui.h"

#include "../../../../Interfaces.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

void Gui::Windows::panorama()
{
	if (ImGui::BeginTabBar("#Panorama debug tabs", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Panels")) {
			Interfaces::panoramaUIEngine->AccessUIEngine()->ForEachPanel([&](CUIPanel* panel) {
				ImGui::Text("%p -> %s", panel, panel->GetID());
			});
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Execute Javascript")) {
			static char panel[512] = "CSGOMainMenu";
			ImGui::InputText("Panel", panel, IM_ARRAYSIZE(panel));
			static char context[512] = "panorama/layout/base.xml";
			ImGui::InputText("Context", context, IM_ARRAYSIZE(context));
			static char source[4096] = "";
			ImGui::InputTextMultiline("Source", source, IM_ARRAYSIZE(source));

			if (ImGui::Button("Execute!")) {
				CUIEngine* uiEngine = Interfaces::panoramaUIEngine->AccessUIEngine();
				CUIPanel* uiPanel = uiEngine->FindPanel(panel);
				uiEngine->RunScript(uiPanel, source, context);
			}

			static bool bypassed = false;
			if (!bypassed && ImGui::Button("Bypass file path limitations")) {
				bypassed = true;
				static const unsigned char returnZero[]{ 0x48, 0x31, 0xc0, 0xc3 };

				char* checkFilePath = static_cast<char*>(BCRL::Session::module("panorama_gl_client.so")
															 .nextStringOccurence("FixupResourceName: Illegal full path passed in (\"%s\")!")
															 .findXREFs("panorama_gl_client.so", true, false)
															 .filter([](const BCRL::SafePointer& safePointer) {
																 return safePointer.add(4).doesMatch("48 89 de");
															 })
															 .prevByteOccurence("48 89 fb e8")
															 .add(4)
															 .relativeToAbsolute()
															 .getPointer()
															 .value());

				const size_t pagesize = getpagesize();
				char* aligned = reinterpret_cast<char*>(reinterpret_cast<std::uintptr_t>(checkFilePath) & ~(pagesize - 1));
				const size_t alignDifference = (char*)checkFilePath - aligned;
				mprotect(aligned, alignDifference + 4, PROT_READ | PROT_WRITE | PROT_EXEC);
				memcpy(checkFilePath, returnZero, 4);
				mprotect(aligned, alignDifference + 4, PROT_READ | PROT_EXEC);
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}
