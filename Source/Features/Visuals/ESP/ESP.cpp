#include "ESP.hpp"

#include "imgui.h"

#include "../../../Interfaces.hpp"

#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/Popup.hpp"

#include "EntityCache/EntityCache.hpp"
#include "ESPStructure/ESPStructure.hpp"

#include <optional>
#include <vector>

void ESP::imGuiRender(ImDrawList* drawList)
{
	if (!enabled || (key.isSet() && !key.isActive()))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	std::optional<LocalPlayer> localPlayer = EntityCache::localPlayer;
	if (!localPlayer.has_value())
		return;

	Vector viewangles;
	if (outOfView) { // We don't need the viewangles otherwise
		Interfaces::engine->GetViewAngles(&viewangles);
	}

	players.draw(drawList, localPlayer.value());

	// Give spectators a lower priority than actual players
	for (auto& [_, spectator] : EntityCache::spectators) {
		if (spectator.dormant)
			continue; // No point in displaying outdated positions

		if (!localPlayer->alive && localPlayer->observerMode == ObserverMode::OBS_MODE_IN_EYE) {
			if (spectator.observerTarget == localPlayer->observerTarget)
				continue;
		} else if (spectator.observerMode == ObserverMode::OBS_MODE_IN_EYE) {
			if (spectator.observerTarget == localPlayer->handle)
				continue;
		}

		spectators.draw(drawList, spectator);
	}

	for (auto& [_, player] : EntityCache::players) {
		if (player.dormant)
			continue; // No point in displaying outdated positions
		players.draw(drawList, player);
	}

	for (auto& [_, weapon] : EntityCache::weapons) {
		if (weapon.ownerEntity == -1)
			weapons.draw(drawList, weapon);
	}

	for (auto& [_, hostage] : EntityCache::hostages) {
		hostages.draw(drawList, hostage);
	}

	for (auto& [_, projectile] : EntityCache::projectiles) {
		projectiles.draw(drawList, projectile);
	}

	for (auto& [_, bomb] : EntityCache::bombs) {
		plantedC4.draw(drawList, bomb);
	}

	for (auto& [_, lootCrate] : EntityCache::lootCrates) {
		dzLootCrates.draw(drawList, lootCrate);
	}

	for (auto& [_, drone] : EntityCache::drones) {
		dzDrones.draw(drawList, drone);
	}

	for (auto& [_, sentry] : EntityCache::sentries) {
		dzSentries.draw(drawList, sentry);
	}

	for (auto& [_, entity] : EntityCache::ammoBoxes) {
		dzAmmoBoxes.draw(drawList, entity, "Ammo box");
	}

	for (auto& [_, entity] : EntityCache::entities) {
		other.draw(drawList, entity, entity.clientClass->m_pNetworkName);
	}
}

void ESP::update()
{
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	EntityCache::updateEntities(
		drawDistance,
		other.isEnabled(),
		players.isEnabled(),
		spectators.isEnabled(),
		weapons.isEnabled(),
		hostages.isEnabled(),
		projectiles.isEnabled(),
		plantedC4.isEnabled(),
		dzAmmoBoxes.isEnabled(),
		dzLootCrates.isEnabled(),
		dzDrones.isEnabled(),
		dzSentries.isEnabled());
}

void ESP::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::SameLine();
	ImGui::SliderInt("Draw distance", &drawDistance, 0, 1024 * 16);

	ImGui::Checkbox("Out of view", &outOfView);
	ImGui::SameLine();
	if (ImGui::Popup("Out of view settings")) {
		ImGui::SliderFloat("Size", &outOfViewSize, 0.0f, 50.0f, "%.2f");
		ImGui::SliderFloat("Distance", &outOfViewDistance, 0.0f, 500.0f, "%.2f");
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Align bounding boxes with the pixel grid", &alignBoundingBox);

	ImGui::Text("Visibility checks");
	ImGui::SameLine();
	if (ImGui::Popup("Visibility checker settings")) {
		visibilityChecker.setupGUI();
		ImGui::EndPopup();
	}

	ImGui::InputSelector("Key (%s)", key);

	if (ImGui::BeginTabBar("#Config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Players")) {
			players.setupGUI("Players");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Spectators")) {
			spectators.setupGUI("Spectators");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapons")) {
			weapons.setupGUI("Weapons");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Projectiles")) {
			projectiles.setupGUI("Projectiles");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Planted C4")) {
			plantedC4.setupGUI("Planted C4");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Hostages")) {
			hostages.setupGUI("Hostages");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Danger Zone")) {
			if (ImGui::BeginTabBar("#Danger Zone config selection", ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem("Loot crates")) {
					dzLootCrates.setupGUI("Loot crates");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Ammo boxes")) {
					dzAmmoBoxes.setupGUI("Ammo boxes");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Sentries")) {
					dzSentries.setupGUI("Sentries");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Drones")) {
					dzDrones.setupGUI("Drones");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Other")) {
			other.setupGUI("Other");
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::Separator();

	ImGui::Text("Entities: %zu", EntityCache::entities.size());
	ImGui::Text("Players: %zu", EntityCache::players.size());
	ImGui::Text("Spectators: %zu", EntityCache::spectators.size());
	ImGui::Text("Weapons: %zu", EntityCache::weapons.size());
	ImGui::Text("Hostages: %zu", EntityCache::hostages.size());
	ImGui::Text("Projectiles: %zu", EntityCache::projectiles.size());
	ImGui::Text("Bombs: %zu", EntityCache::bombs.size());
	ImGui::Text("Loot crates: %zu", EntityCache::lootCrates.size());
	ImGui::Text("Drones: %zu", EntityCache::drones.size());
	ImGui::Text("Sentries: %zu", EntityCache::sentries.size());
}

SCOPED_SERIALIZER(ESP)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Draw distance", drawDistance);
	SERIALIZE_STRUCT("Key", key);

	SERIALIZE_STRUCT("Visibility checker", visibilityChecker);

	SERIALIZE("Out of view", outOfView);
	SERIALIZE("Out of view size", outOfViewSize);
	SERIALIZE("Out of view distance", outOfViewDistance);

	SERIALIZE("Align bounding box", alignBoundingBox);

	SERIALIZE_STRUCT("Players", players);
	SERIALIZE_STRUCT("Spectators", spectators);
	SERIALIZE_STRUCT("Weapons", weapons);
	SERIALIZE_STRUCT("Projectiles", projectiles);
	SERIALIZE_STRUCT("Planted C4", plantedC4);
	SERIALIZE_STRUCT("Hostages", hostages);
	SERIALIZE_STRUCT("Loot crates", dzLootCrates);
	SERIALIZE_STRUCT("Ammo boxes", dzAmmoBoxes);
	SERIALIZE_STRUCT("Sentries", dzSentries);
	SERIALIZE_STRUCT("Drones", dzDrones);
	SERIALIZE_STRUCT("Other", other);
}
