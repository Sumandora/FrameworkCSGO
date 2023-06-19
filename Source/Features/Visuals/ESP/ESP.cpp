#include "ESP.hpp"

#include "imgui.h"
#include "xorstr.hpp"

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
		dzAmmoBoxes.Draw(drawList, entity, xorstr_("Ammo box"));
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
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SameLine();
	ImGui::SliderInt(xorstr_("Draw distance"), &drawDistance, 0, 1024 * 16);

	ImGui::Checkbox(xorstr_("Consider spotted entities as visible"), &considerSpottedEntitiesAsVisible);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr_("Out of view"), &outOfView);
	ImGui::SameLine();
	if (ImGui::Popup(xorstr_("Out of view settings"))) {
		ImGui::SliderFloat(xorstr_("Size"), &outOfViewSize, 0.0f, 50.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Distance"), &outOfViewDistance, 0.0f, 500.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

	ImGui::Checkbox(xorstr_("Consider smoked off entities as occluded"), &considerSmokedOffEntitiesAsOccluded);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr_("Align bounding boxes with the pixel grid"), &alignBoundingBox);

	ImGui::Checkbox(xorstr_("Consider everyone visible when dead"), &considerEveryoneVisibleWhenDead);

	ImGui::InputSelector(xorstr_("Hold key (%s)"), onKey);

	if (ImGui::BeginTabBar(xorstr_("#Config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Players"))) {
			players.SetupGUI(xorstr_("Players"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Spectators"))) {
			spectators.SetupGUI(xorstr_("Spectators"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Weapons"))) {
			weapons.SetupGUI(xorstr_("Weapons"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Projectiles"))) {
			projectiles.SetupGUI(xorstr_("Projectiles"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Planted C4"))) {
			plantedC4.SetupGUI(xorstr_("Planted C4"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Hostages"))) {
			hostages.SetupGUI(xorstr_("Hostages"));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Danger Zone"))) {
			if (ImGui::BeginTabBar(xorstr_("#Danger Zone config selection"), ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem(xorstr_("Loot crates"))) {
					dzLootCrates.SetupGUI(xorstr_("Loot crates"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Ammo boxes"))) {
					dzAmmoBoxes.SetupGUI(xorstr_("Ammo boxes"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Sentries"))) {
					dzSentries.SetupGUI(xorstr_("Sentries"));
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Drones"))) {
					dzDrones.SetupGUI(xorstr_("Drones"));
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Other"))) {
			other.SetupGUI(xorstr_("Other"));
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::Separator();

	ImGui::Text(xorstr_("Entities: %d"), EntityCache::entities.size());
	ImGui::Text(xorstr_("Players: %d"), EntityCache::players.size());
	ImGui::Text(xorstr_("Spectators: %d"), EntityCache::spectators.size());
	ImGui::Text(xorstr_("Weapons: %d"), EntityCache::weapons.size());
	ImGui::Text(xorstr_("Hostages: %d"), EntityCache::hostages.size());
	ImGui::Text(xorstr_("Projectiles: %d"), EntityCache::projectiles.size());
	ImGui::Text(xorstr_("Bombs: %d"), EntityCache::bombs.size());
	ImGui::Text(xorstr_("Loot crates: %d"), EntityCache::lootCrates.size());
	ImGui::Text(xorstr_("Drones: %d"), EntityCache::drones.size());
	ImGui::Text(xorstr_("Sentries: %d"), EntityCache::sentries.size());
}

SCOPED_SERIALIZER(ESP)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE(xorstr_("Draw distance"), drawDistance);
	SERIALIZE(xorstr_("Hold key"), onKey);

	SERIALIZE(xorstr_("Consider spotted entities as visible"), considerSpottedEntitiesAsVisible);
	SERIALIZE(xorstr_("Consider smoked off entities as occluded"), considerSmokedOffEntitiesAsOccluded);
	SERIALIZE(xorstr_("Consider everyone visible when dead"), considerEveryoneVisibleWhenDead);

	SERIALIZE(xorstr_("Out of view"), outOfView);
	SERIALIZE(xorstr_("Out of view size"), outOfViewSize);
	SERIALIZE(xorstr_("Out of view distance"), outOfViewDistance);

	SERIALIZE(xorstr_("Align bounding box"), alignBoundingBox);

	SERIALIZE_STRUCT(xorstr_("Players"), players);
	SERIALIZE_STRUCT(xorstr_("Spectators"), spectators);
	SERIALIZE_STRUCT(xorstr_("Weapons"), weapons);
	SERIALIZE_STRUCT(xorstr_("Projectiles"), projectiles);
	SERIALIZE_STRUCT(xorstr_("Planted C4"), plantedC4);
	SERIALIZE_STRUCT(xorstr_("Hostages"), hostages);
	SERIALIZE_STRUCT(xorstr_("Loot crates"), dzLootCrates);
	SERIALIZE_STRUCT(xorstr_("Ammo boxes"), dzAmmoBoxes);
	SERIALIZE_STRUCT(xorstr_("Sentries"), dzSentries);
	SERIALIZE_STRUCT(xorstr_("Drones"), dzDrones);
	SERIALIZE_STRUCT(xorstr_("Other"), other);
}
