#include "../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../GameCache.hpp"
#include "../../../Interfaces.hpp"

#include "../../../GUI/Elements/HelpMarker.hpp"
#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/Popup.hpp"

#include "../../../Hooks/Game/GameFunctions.hpp"

#include "../../../GUI/ImGuiColors.hpp"
#include "../../../Utils/Raytrace.hpp"
#include "../../../Utils/Trigonometry.hpp"

#include <vector>

static bool enabled = false;
static int onKey = ImGuiKey_None;
static int drawDistance = 1024 * 8;
static bool considerSpottedEntitiesAsVisible = false;
bool Features::Visuals::Esp::considerSmokedOffEntitiesAsOccluded = true;
static bool considerEveryoneVisibleWhenDead = false;
static bool sortEntitiesByDistance = true;
static bool alignBoundingBox = true;
static bool outOfView = false;
static float outOfViewSize = 30.0f;
static float outOfViewDistance = 300.0f;
PlayerSettings Features::Visuals::Esp::players;
static WeaponSettings weapons;
static ProjectileSettings projectiles;
static PlantedC4Settings plantedC4;
static BoxNameSetting hostages;
static BoxNameSetting dzLootCrates;
static BoxNameSetting dzAmmoBoxes;
static BoxNameSetting dzSentries;
static BoxNameSetting other;
// TODO Drone ESP

static std::map<const char*, const char*> lootCrateNames{
	{ strdup(xorstr_("case_pistol.mdl")), strdup(xorstr_("Pistol Case")) },
	{ strdup(xorstr_("case_light_weapon.mdl")), strdup(xorstr_("Light Case")) },
	{ strdup(xorstr_("case_heavy_weapon.mdl")), strdup(xorstr_("Heavy Case")) },
	{ strdup(xorstr_("case_explosive.mdl")), strdup(xorstr_("Explosive Case")) },
	{ strdup(xorstr_("case_tools.mdl")), strdup(xorstr_("Tools Case")) },
	{ strdup(xorstr_("dufflebag.mdl")), strdup(xorstr_("Cash Dufflebag")) }
};

bool Features::Visuals::Esp::WorldToScreen(const Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition)
{
	const float z = matrix[2][0] * worldPosition.x + matrix[2][1] * worldPosition.y + matrix[2][2] * worldPosition.z + matrix[2][3];
	const float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
	if (z <= 0.0f || w <= 0.0f)
		return false;

	screenPosition = ImGui::GetIO().DisplaySize;
	screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;

	screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
	screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
	return true;
}

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

bool CalculateScreenRectangle(const Vector& origin, const BoundingBox& boundingBox, ImVec4& rectangle)
{
	const Vector min = origin + boundingBox.mins;
	const Vector max = origin + boundingBox.maxs;

	const Vector points[] = {
		// Lower
		Vector(min.x, min.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, min.y, max.z),
		Vector(min.x, min.y, max.z),
		// Higher
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z)
	};

	rectangle = { FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (const auto& point : points) {
		ImVec2 point2D;

		if (!Features::Visuals::Esp::WorldToScreen(Hooks::Game::FrameStageNotify::worldToScreenMatrix, point, point2D)) {
			return false;
		}

		if (point2D.x < rectangle.x)
			rectangle.x = point2D.x;
		else if (point2D.x > rectangle.z)
			rectangle.z = point2D.x;

		if (point2D.y < rectangle.y)
			rectangle.y = point2D.y;
		else if (point2D.y > rectangle.w)
			rectangle.w = point2D.y;
	}

	if (alignBoundingBox) {
		rectangle.x = std::round(rectangle.x);
		rectangle.y = std::round(rectangle.y);
		rectangle.z = std::round(rectangle.z);
		rectangle.w = std::round(rectangle.w);
	}

	return true;
}

bool HandleOutOfView(const Vector& localOrigin, const Vector& otherOrigin, const Vector& viewangles, ImVec4& rectangle)
{
	if (outOfView) {
		const Vector delta = otherOrigin - localOrigin;
		const float angle = (float)DEG2RAD(viewangles.y - 90.0f) - atan2f(delta.y, delta.x);

		const ImVec2 display = ImGui::GetIO().DisplaySize;
		const ImVec2 direction(cosf(angle), sinf(angle));

		const ImVec2 screenPosition(
			(float)display.x / 2.0f + direction.x * outOfViewDistance,
			(float)display.y / 2.0f + direction.y * outOfViewDistance);

		rectangle = ImVec4(
			screenPosition.x - outOfViewSize,
			screenPosition.y - outOfViewSize,

			screenPosition.x + outOfViewSize,
			screenPosition.y + outOfViewSize);

		return true;
	}
	return false;
}

bool ScreenRectangle(ImVec4& rectangle, const Entity& entity, const LocalPlayer& localPlayer, bool outOfView = true)
{
	const std::optional<BoundingBox>& boundingBox = entity.boundingBox;

	if (!boundingBox.has_value())
		return false;

	bool visible = CalculateScreenRectangle(entity.origin, boundingBox.value(), rectangle);

	if (outOfView && !visible && HandleOutOfView(localPlayer.origin, entity.origin, localPlayer.viewangles, rectangle)) { // TODO Buy menu makes oov flicker
		visible = true; // We just made them visible ^^
	}

	return visible;
}

void DrawLocalPlayer(ImDrawList* drawList, const LocalPlayer& localPlayer)
{
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, localPlayer, localPlayer, false))
		return;

	Features::Visuals::Esp::players.local.Draw(drawList, rectangle, localPlayer);
}

void DrawPlayer(ImDrawList* drawList, const Player& player, const LocalPlayer& localPlayer)
{
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, player, localPlayer))
		return;

	PlayerStateSettings* settings;

	if (!player.enemy)
		settings = SelectPlayerState(localPlayer, player, &Features::Visuals::Esp::players.teammate);
	else
		settings = SelectPlayerState(localPlayer, player, &Features::Visuals::Esp::players.enemy);

	if (settings)
		settings->Draw(drawList, rectangle, player);
}

void DrawSpectator(ImDrawList* drawList, const Player& player, const LocalPlayer& localPlayer)
{
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, player, localPlayer))
		return;

	char name[128];
	if (Features::Visuals::Esp::players.spectators.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		PlayerInfo info{};
		Interfaces::engine->GetPlayerInfo(player.index, &info);
		strcpy(name, info.name);
	}
	Features::Visuals::Esp::players.spectators.Draw(drawList, rectangle, name);
}

void DrawWeapon(ImDrawList* drawList, const Weapon& weapon, const LocalPlayer& localPlayer) {
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, weapon, localPlayer))
		return;

	if (weapon.ownerEntity == -1)
		weapons.Draw(drawList, rectangle, weapon);
}

void DrawProjectile(ImDrawList* drawList, const Projectile& projectile, const LocalPlayer& localPlayer) {
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, projectile, localPlayer))
		return;

	projectiles.Draw(drawList, rectangle, projectile);
}

void DrawPlantedC4(ImDrawList* drawList, const PlantedC4& bomb, const LocalPlayer& localPlayer) {
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, bomb, localPlayer))
		return;

	plantedC4.Draw(drawList, rectangle, bomb);
}

void DrawEntity(ImDrawList* drawList, const Entity& entity, const LocalPlayer& localPlayer) {
	ImVec4 rectangle;
	if(!ScreenRectangle(rectangle, entity, localPlayer))
		return;

	switch (entity.clientClass->m_ClassID) {
	case ClientClassID::CHostage:
		hostages.Draw(drawList, rectangle, xorstr_("Hostage"));
		break;
	case ClientClassID::CPhysPropLootCrate: {
		const char* name = xorstr_("Unknown loot crate");
		if(dzLootCrates.nametag.enabled) {
			const char* modelName = entity.model->szPathName;
			for (const auto pair : lootCrateNames) {
				if (strstr(modelName, pair.first)) {
					name = pair.second;
				}
			}
		}

		dzLootCrates.Draw(drawList, rectangle, name);
		break;
	}
	case ClientClassID::CPhysPropAmmoBox:
		dzAmmoBoxes.Draw(drawList, rectangle, xorstr_("Ammo box"));
		break;
	case ClientClassID::CDronegun:
		dzSentries.Draw(drawList, rectangle, xorstr_("Sentry"));
		break;
	default:
		other.Draw(drawList, rectangle, entity.clientClass->m_pNetworkName);
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

	DrawLocalPlayer(drawList, localPlayer.value());

	for (const Player& player : EntityCache::GetPlayers()) {
		DrawPlayer(drawList, player, localPlayer.value());
	}

	for (const Player& spectator : EntityCache::GetSpectators()) {
		DrawSpectator(drawList, spectator, localPlayer.value());
	}

	for(const Weapon& weapon : EntityCache::GetWeapons()) {
		DrawWeapon(drawList, weapon, localPlayer.value());
	}

	for(const Projectile& projectile : EntityCache::GetProjectiles()) {
		DrawProjectile(drawList, projectile, localPlayer.value());
	}

	for(const PlantedC4& bomb : EntityCache::GetBombs()) {
		DrawPlantedC4(drawList, bomb, localPlayer.value());
	}

	for(const Entity& entity : EntityCache::GetEntities()) {
		DrawEntity(drawList, entity, localPlayer.value());
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
	ImGui::Checkbox(xorstr_("Distance-based rendering"), &sortEntitiesByDistance);

	ImGui::Checkbox(xorstr_("Consider everyone visible when dead"), &considerEveryoneVisibleWhenDead);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr_("Align bounding boxes with the pixel grid"), &alignBoundingBox);

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

SERIALIZED_TYPE(xorstr_("Sort entities by distance"), sortEntitiesByDistance)
SERIALIZED_TYPE(xorstr_("Align bounding box"), alignBoundingBox)

SERIALIZED_STRUCTURE(xorstr_("Players"), players)
SERIALIZED_STRUCTURE(xorstr_("Weapons"), weapons)
SERIALIZED_STRUCTURE(xorstr_("Projectiles"), projectiles)
SERIALIZED_STRUCTURE(xorstr_("Planted C4"), plantedC4)
SERIALIZED_STRUCTURE(xorstr_("Hostages"), hostages)
SERIALIZED_STRUCTURE(xorstr_("Loot crates"), dzLootCrates)
SERIALIZED_STRUCTURE(xorstr_("Ammo boxes"), dzAmmoBoxes)
SERIALIZED_STRUCTURE(xorstr_("Sentries"), dzSentries)
SERIALIZED_STRUCTURE(xorstr_("Other"), other)
END_SERIALIZED_STRUCT
