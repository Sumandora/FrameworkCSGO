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

// Returns false when the glow wasn't applied
bool Glow::updateGlow(int entindex, CBaseEntity* entity) const
{
	if (!entity || entity->GetDormant())
		return false;
	if (entity->IsPlayer()) {
		auto* player = static_cast<CBasePlayer*>(entity);
		if (player->IsAlive()) {
			return players.apply(entindex, player);
		}
		return false;
	} else if (entity->IsWeaponWorldModel()) {
		return weapons.apply(entindex, entity);
	} else {
		ClientClass* clientClass = entity->GetClientClass();
		if (clientClass->m_ClassID == ClientClassID::CCSRagdoll) {
			return ragdolls.apply(entindex, entity);
		}
		return false;
	}
}

void Glow::updateGlow()
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
		if (!updateGlow(i, entity)) {
			auto it = customGlows.find(i);
			if (it != customGlows.end()) {
				Memory::glowObjectManager->UnregisterGlowObject(it->second);
				customGlows.erase(it);
			}
		}
	}
}

void Glow::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);

	if (ImGui::BeginTabBar("#Config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Players")) {
			players.setupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapons")) {
			weapons.setupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Ragdolls")) {
			ragdolls.setupGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

SCOPED_SERIALIZER(Glow)
{
	SERIALIZE("Enabled", enabled);

	SERIALIZE_STRUCT("Players", players);
	SERIALIZE_STRUCT("Weapons", weapons);
	SERIALIZE_STRUCT("Ragdolls", ragdolls);
}