#include "../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../Interfaces.hpp"

#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/Popup.hpp"

#include <vector>

static bool enabled = false;
static int onKey = ImGuiKey_None;
static int drawDistance = 1024 * 8;
static bool considerSpottedEntitiesAsVisible = false;
bool Features::Visuals::Esp::considerSmokedOffEntitiesAsOccluded = true;
static bool considerEveryoneVisibleWhenDead = false;
bool Features::Visuals::Esp::alignBoundingBox = true;
bool Features::Visuals::Esp::outOfView = false;
float Features::Visuals::Esp::outOfViewSize = 30.0f;
float Features::Visuals::Esp::outOfViewDistance = 300.0f;
PlayerSettings Features::Visuals::Esp::players;
static WeaponSettings weapons;
static ProjectileSettings projectiles;
static PlantedC4Settings plantedC4;
static HostageSettings hostages;
static LootCrateTypeSettings dzLootCrates;
static GenericEntitySettings dzAmmoBoxes;
static SentrySettings dzSentries;
static DroneSettings dzDrones;
static GenericEntitySettings other;
// TODO Drone ESP

PlayerStateSettings* SelectPlayerState(const LocalPlayer& localPlayer, const Player& player, PlayerTeamSettings* settings)
{
	if (settings->visible == settings->occluded)
		return &settings->visible; // Having visible == occluded is a common configuration, we can skip most of this function if it is the case

	if (considerSpottedEntitiesAsVisible && player.spotted)
		return &settings->visible; // Don't even have to raytrace for that.

	if (considerEveryoneVisibleWhenDead && !localPlayer.alive)
		return &settings->visible;

	if (player.visible)
		return &settings->visible;
	else
		return &settings->occluded;
}

void DrawPlayer(ImDrawList* drawList, Player& player, const LocalPlayer& localPlayer)
{
	PlayerStateSettings* settings;

	if (!player.enemy)
		settings = SelectPlayerState(localPlayer, player, &Features::Visuals::Esp::players.teammate);
	else
		settings = SelectPlayerState(localPlayer, player, &Features::Visuals::Esp::players.enemy);

	if (settings)
		settings->Draw(drawList, player);
}

void DrawSpectator(ImDrawList* drawList, Spectator& player)
{
	// TODO Hide if spectated entity isnt dormant
	// TODO Show name of spectated entity
	char name[MAX_NAME_LEN];

	if (Features::Visuals::Esp::players.spectators.boxName.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		PlayerInfo info{};
		Interfaces::engine->GetPlayerInfo(player.index, &info);
		strcpy(name, info.name);
	}

	Features::Visuals::Esp::players.spectators.Draw(drawList, player, name);
}

void DrawEntity(ImDrawList* drawList, Entity& entity)
{
	switch (entity.clientClass->m_ClassID) {
	case ClientClassID::CPhysPropAmmoBox:
		dzAmmoBoxes.Draw(drawList, entity, xorstr_("Ammo box"));
		break;
	default:
		other.Draw(drawList, entity, entity.clientClass->m_pNetworkName);
		break;
	}
}

void Features::Visuals::Esp::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	std::optional<LocalPlayer> localPlayer = EntityCache::GetLocalPlayer();
	if (!localPlayer.has_value())
		return;

	Vector viewangles;
	if (outOfView) { // We don't need the viewangles otherwise
		Interfaces::engine->GetViewAngles(&viewangles);
	}

	Features::Visuals::Esp::players.local.Draw(drawList, localPlayer.value());

	// Give spectators a lower priority than actual players
	for (Spectator& spectator : EntityCache::GetSpectators()) {
		if (!localPlayer->alive && localPlayer->observerMode == ObserverMode::OBS_MODE_IN_EYE) {
			if (spectator.observerTarget == localPlayer->observerTarget)
				continue;
		} else if (spectator.observerMode == ObserverMode::OBS_MODE_IN_EYE) {
			if (spectator.observerTarget == localPlayer->handle)
				continue;
		}
		DrawSpectator(drawList, spectator);
	}

	for (Player& player : EntityCache::GetPlayers()) {
		DrawPlayer(drawList, player, localPlayer.value());
	}

	for (Weapon& weapon : EntityCache::GetWeapons()) {
		if (weapon.ownerEntity == -1)
			weapons.Draw(drawList, weapon);
	}

	for (Hostage& hostage : EntityCache::GetHostages()) {
		hostages.Draw(drawList, hostage);
	}

	for (Projectile& projectile : EntityCache::GetProjectiles()) {
		projectiles.Draw(drawList, projectile);
	}

	for (PlantedC4& bomb : EntityCache::GetBombs()) {
		plantedC4.Draw(drawList, bomb);
	}

	for(LootCrate& lootCrate : EntityCache::GetLootCrates()) {
		dzLootCrates.Draw(drawList, lootCrate);
	}

	for(Drone& drone : EntityCache::GetDrones()) {
		dzDrones.Draw(drawList, drone);
	}

	for(Sentry& sentry : EntityCache::GetSentries()) {
		dzSentries.Draw(drawList, sentry);
	}

	for (Entity& entity : EntityCache::GetEntities()) {
		DrawEntity(drawList, entity);
	}
}

void Features::Visuals::Esp::Update()
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	EntityCache::UpdateEntities(drawDistance);
}

void Features::Visuals::Esp::SetupGUI()
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

	ImGui::Text(xorstr_("Entities: %d"), EntityCache::GetEntities().size());
	ImGui::Text(xorstr_("Players: %d"), EntityCache::GetPlayers().size());
	ImGui::Text(xorstr_("Spectators: %d"), EntityCache::GetSpectators().size());
	ImGui::Text(xorstr_("Weapons: %d"), EntityCache::GetWeapons().size());
	ImGui::Text(xorstr_("Hostages: %d"), EntityCache::GetHostages().size());
	ImGui::Text(xorstr_("Projectiles: %d"), EntityCache::GetProjectiles().size());
	ImGui::Text(xorstr_("Bombs: %d"), EntityCache::GetBombs().size());
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::Esp::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Draw distance"), drawDistance)
SERIALIZED_TYPE(xorstr_("Hold key"), onKey)

SERIALIZED_TYPE(xorstr_("Consider spotted entities as visible"), considerSpottedEntitiesAsVisible)
SERIALIZED_TYPE(xorstr_("Consider smoked off entities as occluded"), considerSmokedOffEntitiesAsOccluded)
SERIALIZED_TYPE(xorstr_("Consider everyone visible when dead"), considerEveryoneVisibleWhenDead)

SERIALIZED_TYPE(xorstr_("Out of view"), outOfView)
SERIALIZED_TYPE(xorstr_("Out of view size"), outOfViewSize)
SERIALIZED_TYPE(xorstr_("Out of view distance"), outOfViewDistance)

SERIALIZED_TYPE(xorstr_("Align bounding box"), alignBoundingBox)

SERIALIZED_STRUCTURE(xorstr_("Players"), players)
SERIALIZED_STRUCTURE(xorstr_("Weapons"), weapons)
SERIALIZED_STRUCTURE(xorstr_("Projectiles"), projectiles)
SERIALIZED_STRUCTURE(xorstr_("Planted C4"), plantedC4)
SERIALIZED_STRUCTURE(xorstr_("Hostages"), hostages)
SERIALIZED_STRUCTURE(xorstr_("Loot crates"), dzLootCrates)
SERIALIZED_STRUCTURE(xorstr_("Ammo boxes"), dzAmmoBoxes)
SERIALIZED_STRUCTURE(xorstr_("Sentries"), dzSentries)
SERIALIZED_STRUCTURE(xorstr_("Drones"), dzDrones)
SERIALIZED_STRUCTURE(xorstr_("Other"), other)
END_SERIALIZED_STRUCT
