#include "../GUI.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Features/Features.hpp"

#include "Windows/Configuration.hpp"
#include "Windows/Debug.hpp"

#include "../ImGuiMacros.hpp"
#include <cstring>

void Gui::Build()
{
	ImGui::SetNextWindowSize({ 800, 600 }, ImGuiCond_Once);
	if (ImGui::Begin(xorstr_("Framework CSGO"))) {
		TABBAR(xorstr_("#Top level tabs"), []() {
			for (auto& [category, vector] : features) {
				TABITEM(category.c_str(), [&]() {
					TABBAR(strcat(xorstr_("#"), category.c_str()), [vector]() {
						for (Feature* feature : vector) {
							TABITEM(feature->GetName().c_str(), feature->SetupGUI); // Wrap the member function
						}
					})
				})
			}

			TABITEM(xorstr_("Configuration"), Gui::Windows::Configuration)

			TABITEM(xorstr_("Debug"), []() {
				TABBAR(xorstr_("#Debug tabs"), []() {
					TABITEM(xorstr_("Link maps"), Gui::Windows::LinkMaps)
					TABITEM(xorstr_("Networked vars"), Gui::Windows::Netvars)
					TABITEM(xorstr_("Interfaces"), Gui::Windows::Interfaces)
					TABITEM(xorstr_("Game events"), Gui::Windows::GameEvents)
				})
			})
		})
	}
	ImGui::End();
}