#include "../GUI.hpp"

#include "imgui.h"

#include "../../Features/Features.hpp"

#include "Windows/Configuration.hpp"
#include "Windows/Debug.hpp"

#include <cstring>

void Gui::build()
{
	ImGui::SetNextWindowSize({ 800, 600 }, ImGuiCond_Once);
	if (ImGui::Begin("Framework CSGO")) {
		if (ImGui::BeginTabBar("#Top level tabs", ImGuiTabBarFlags_Reorderable)) {
			for (auto& [category, vector] : features) {
				if (ImGui::BeginTabItem(category.c_str())) {
					std::string tag = category.c_str();
					tag.insert(0, 1, '#');
					if (ImGui::BeginTabBar(tag.c_str(), ImGuiTabBarFlags_Reorderable)) {
						for (Feature* feature : vector) {
							if (ImGui::BeginTabItem(feature->getName().c_str())) {
								feature->setupGUI();
								ImGui::EndTabItem();
							}
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}
			}

			if (ImGui::BeginTabItem("Configuration")) {
				Gui::Windows::configuration();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Debug")) {
				if (ImGui::BeginTabBar("#Debug tabs", ImGuiTabBarFlags_Reorderable)) {
					if (ImGui::BeginTabItem("Link maps")) {
						Gui::Windows::linkMaps();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Networked vars")) {
						Gui::Windows::netvars();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Interfaces")) {
						Gui::Windows::interfaces();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Game events")) {
						Gui::Windows::gameEvents();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Panorama")) {
						Gui::Windows::panorama();
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Entities")) {
						Gui::Windows::entities();
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}