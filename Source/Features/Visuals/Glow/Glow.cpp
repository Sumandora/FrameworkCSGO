#include "Glow.hpp"

#include "../../../GUI/Elements/Popup.hpp"

#include "imgui.h"

#include "../../../Interfaces.hpp"

#include "../../../SDK/GameClass/CGlowObjectManager.hpp"

#include "../../../Hooks/SDL/SDLFunctions.hpp"
#include <unordered_map>

Glow::~Glow()
{
	if (Hooks::SDL::shuttingDown)
		return;
	for (const auto& [entindex, glowIndex] : customGlows) {
		Memory::glowObjectManager->UnregisterGlowObject(glowIndex);
	}
}

void Glow::UpdateGlow()
{
	if (!Interfaces::engine->IsInGame() || !enabled) {
		for (const auto& [entindex, glowIndex] : customGlows) {
			Memory::glowObjectManager->UnregisterGlowObject(glowIndex);
		}
		customGlows.clear();
		return;
	}

	for (int i = 0; i < Interfaces::entityList->GetHighestEntityIndex(); i++) {
		auto* entity = Interfaces::entityList->GetClientEntity(i);
		if (entity && !entity->GetDormant()) {
			if (entity->IsPlayer()) {
				auto* player = static_cast<CBasePlayer*>(entity);
				if (player->IsAlive())
					playerSettings.Apply(i, player);
			} else if (entity->IsWeaponWorldModel()) {
				weapons.Apply(i, entity);
			} else {
				ClientClass* clientClass = entity->GetClientClass();
				if (clientClass->m_ClassID == ClientClassID::CCSRagdoll) {
					ragdolls.Apply(i, entity);
				}
			}
		} else {
			auto it = customGlows.find(i);
			if (it != customGlows.end()) {
				Memory::glowObjectManager->UnregisterGlowObject(it->second);
				customGlows.erase(it);
			}
		}
	}
}

void Glow::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Text("Visibility checks");
	ImGui::SameLine();
	if (ImGui::Popup("Visibility checker settings")) {
		visibilityChecker.SetupGUI();
		ImGui::EndPopup();
	}

	if (ImGui::BeginTabBar("#Config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Players")) {
			playerSettings.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapons")) {
			weapons.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Ragdolls")) {
			ragdolls.SetupGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

SCOPED_SERIALIZER(Glow)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_STRUCT("Visibility checker", visibilityChecker);

	SERIALIZE_STRUCT("Player settings", playerSettings);
}