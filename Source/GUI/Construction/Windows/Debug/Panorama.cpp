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

void Gui::Windows::Panorama()
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

				char* checkFilePath = static_cast<char*>(BCRL::Session::Module("panorama_gl_client.so")
															 .NextStringOccurence("FixupResourceName: Illegal full path passed in (\"%s\")!")
															 .FindXREFs("panorama_gl_client.so", true, false)
															 .Filter([](const BCRL::SafePointer& safePointer) {
																 return safePointer.Add(4).DoesMatch("48 89 de");
															 })
															 .PrevByteOccurence("48 89 fb e8")
															 .Add(4)
															 .RelativeToAbsolute()
															 .Pointer()
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
