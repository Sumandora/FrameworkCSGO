#include "ESP.hpp"

#include "imgui.h"

#include "../../../Interfaces.hpp"

#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/Popup.hpp"

#include "EntityCache/EntityCache.hpp"
#include "ESPStructure/ESPStructure.hpp"

#include <optional>
#include <vector>

void ESP::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !IsInputDown(onKey, true, std::nullopt))
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

	players.Draw(drawList, localPlayer.value());

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

		spectators.Draw(drawList, spectator);
	}

	for (auto& [_, player] : EntityCache::players) {
		if (player.dormant)
			continue; // No point in displaying outdated positions
		players.Draw(drawList, player);
	}

	for (auto& [_, weapon] : EntityCache::weapons) {
		if (weapon.ownerEntity == -1)
			weapons.Draw(drawList, weapon);
	}

	for (auto& [_, hostage] : EntityCache::hostages) {
		hostages.Draw(drawList, hostage);
	}

	for (auto& [_, projectile] : EntityCache::projectiles) {
		projectiles.Draw(drawList, projectile);
	}

	for (auto& [_, bomb] : EntityCache::bombs) {
		plantedC4.Draw(drawList, bomb);
	}

	for (auto& [_, lootCrate] : EntityCache::lootCrates) {
		dzLootCrates.Draw(drawList, lootCrate);
	}

	for (auto& [_, drone] : EntityCache::drones) {
		dzDrones.Draw(drawList, drone);
	}

	for (auto& [_, sentry] : EntityCache::sentries) {
		dzSentries.Draw(drawList, sentry);
	}

	for (auto& [_, entity] : EntityCache::ammoBoxes) {
		dzAmmoBoxes.Draw(drawList, entity, "Ammo box");
	}

	for (auto& [_, entity] : EntityCache::entities) {
		other.Draw(drawList, entity, entity.clientClass->m_pNetworkName);
	}
}

void ESP::Update()
{
	if (!enabled || !IsInputDown(onKey, true, std::nullopt))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	EntityCache::UpdateEntities(
		drawDistance,
		other.IsEnabled(),
		players.teammate.IsEnabled() || players.enemy.IsEnabled() || players.teammate.IsEnabled(),
		spectators.IsEnabled(),
		weapons.IsEnabled(),
		hostages.IsEnabled(),
		projectiles.IsEnabled(),
		plantedC4.IsEnabled(),
		dzAmmoBoxes.IsEnabled(),
		dzLootCrates.IsEnabled(),
		dzDrones.IsEnabled(),
		dzSentries.IsEnabled());
}

void ESP::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::SameLine();
	ImGui::SliderInt("Draw distance", &drawDistance, 0, 1024 * 16);

	ImGui::Checkbox("Consider spotted entities as visible", &considerSpottedEntitiesAsVisible);
	ImGui::SameLine();
	ImGui::Checkbox("Out of view", &outOfView);
	ImGui::SameLine();
	if (ImGui::Popup("Out of view settings")) {
		ImGui::SliderFloat("Size", &outOfViewSize, 0.0f, 50.0f, "%.2f");
		ImGui::SliderFloat("Distance", &outOfViewDistance, 0.0f, 500.0f, "%.2f");
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Consider smoked off entities as occluded", &considerSmokedOffEntitiesAsOccluded);
	ImGui::SameLine();
	ImGui::Checkbox("Align bounding boxes with the pixel grid", &alignBoundingBox);

	ImGui::Checkbox("Consider everyone visible when dead", &considerEveryoneVisibleWhenDead);

	ImGui::InputSelector("Hold key (%s)", onKey);

	if (ImGui::BeginTabBar("#Config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Players")) {
			players.SetupGUI("Players");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Spectators")) {
			spectators.SetupGUI("Spectators");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Weapons")) {
			weapons.SetupGUI("Weapons");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Projectiles")) {
			projectiles.SetupGUI("Projectiles");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Planted C4")) {
			plantedC4.SetupGUI("Planted C4");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Hostages")) {
			hostages.SetupGUI("Hostages");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Danger Zone")) {
			if (ImGui::BeginTabBar("#Danger Zone config selection", ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem("Loot crates")) {
					dzLootCrates.SetupGUI("Loot crates");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Ammo boxes")) {
					dzAmmoBoxes.SetupGUI("Ammo boxes");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Sentries")) {
					dzSentries.SetupGUI("Sentries");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Drones")) {
					dzDrones.SetupGUI("Drones");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Other")) {
			other.SetupGUI("Other");
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
	SERIALIZE("Hold key", onKey);

	SERIALIZE("Consider spotted entities as visible", considerSpottedEntitiesAsVisible);
	SERIALIZE("Consider smoked off entities as occluded", considerSmokedOffEntitiesAsOccluded);
	SERIALIZE("Consider everyone visible when dead", considerEveryoneVisibleWhenDead);

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
