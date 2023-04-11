#include "../../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../GameCache.hpp"
#include "../../../Interfaces.hpp"

#include "../../../GUI/Elements/HelpMarker.hpp"
#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/Popup.hpp"

#include "../../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include "../../../Utils/Raytrace.hpp"
#include "../../../Utils/Trigonometry.hpp"

#include <cstdint>
#include <vector>

static bool enabled = false;
static int onKey = 0;
static int drawDistance = 1024 * 8;
static bool considerSpottedEntitiesAsVisible = false;
static bool considerSmokedOffEntitiesAsOccluded = true;
static bool cacheVisibilityState = true;
static bool outOfView = false;
static float outOfViewSize = 30.0f;
static float outOfViewDistance = 300.0f;
PlayerSettings Features::Visuals::Esp::players;
static WeaponSettings weapons;
static BoxNameSetting projectiles;
static PlantedC4Settings plantedC4;
static BoxNameSetting hostages;
static BoxNameSetting dzLootCrates;
static BoxNameSetting dzAmmoBoxes;
static BoxNameSetting dzSentries;
static BoxNameSetting other;
// TODO Drone ESP

static std::map<int, bool> visibilityCache;

bool Features::Visuals::Esp::WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition)
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

bool IsVisible(CBasePlayer* localPlayer, CBasePlayer* otherPlayer)
{
	Matrix3x4* boneMatrix = otherPlayer->SetupBones();

	const Vector playerEye = localPlayer->GetEyePosition();
	const Vector head = boneMatrix[8].Origin();

	if (considerSmokedOffEntitiesAsOccluded && Memory::LineGoesThroughSmoke(playerEye, head, 1))
		return false;

	CTraceFilterEntity filter(localPlayer);
	Trace trace = Utils::TraceRay(playerEye, head, &filter);

	return trace.m_pEnt == otherPlayer;
}

PlayerStateSettings* SelectPlayerState(CBasePlayer* localPlayer, CBasePlayer* player, PlayerTeamSettings* settings)
{
	if (player->GetDormant())
		return &settings->dormant;

	if (settings->visible == settings->occluded)
		return &settings->visible; // Having visible == occluded is a common configuration, we can skip most of this function if it is the case

	if (considerSpottedEntitiesAsVisible && *player->Spotted())
		return &settings->visible; // Don't even have to raytrace for that.

	bool visible;
	if (cacheVisibilityState)
		visible = visibilityCache[player->entindex()];
	else
		visible = IsVisible(localPlayer, player);

	if (visible)
		return &settings->visible;
	else
		return &settings->occluded;
}

void Features::Visuals::Esp::UpdateVisibility()
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player->GetDormant() || player == localPlayer)
			continue;

		visibilityCache[i] = IsVisible(localPlayer, player);
	}
}

void Features::Visuals::Esp::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer)
		return;

	Vector viewangles;
	if (outOfView) { // We don't need the viewangles otherwise
		Interfaces::engine->GetViewAngles(&viewangles);
	}

	CBaseEntity* spectatorEntity = nullptr;
	if (*localPlayer->ObserverMode() == ObserverMode::OBS_MODE_IN_EYE && localPlayer->ObserverTarget()) {
		spectatorEntity = Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ObserverTarget());
	}

	Matrix4x4 matrix = Hooks::FrameStageNotify::worldToScreenMatrix;

	if (!matrix.Base())
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::entityList->GetHighestEntityIndex(); i++) {
		auto* entity = Interfaces::entityList->GetClientEntity(i);
		if (!entity)
			continue;

		CCollideable* collideable = entity->Collision();

		if (!collideable)
			continue;

		if ((*entity->Origin() - *localPlayer->Origin()).LengthSquared() > (float)(drawDistance * drawDistance))
			continue;

		const Vector min = *entity->Origin() + *collideable->ObbMins();
		const Vector max = *entity->Origin() + *collideable->ObbMaxs();

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

		ImVec4 rectangle(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
		bool visible = true;

		for (const auto& point : points) {
			ImVec2 point2D;

			if (!WorldToScreen(matrix, point, point2D)) {
				visible = false;
				break;
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

		if (outOfView && !visible) {
			Vector delta = *entity->Origin() - *localPlayer->Origin();
			float angle = (float)DEG2RAD(viewangles.y - 90.0f) - atan2f(delta.y, delta.x);

			ImVec2 display = ImGui::GetIO().DisplaySize;
			ImVec2 direction(cosf(angle), sinf(angle));

			ImVec2 screenPosition(
				(float)display.x / 2.0f + direction.x * outOfViewDistance,
				(float)display.y / 2.0f + direction.y * outOfViewDistance);

			rectangle = ImVec4(
				screenPosition.x - outOfViewSize,
				screenPosition.y - outOfViewSize,

				screenPosition.x + outOfViewSize,
				screenPosition.y + outOfViewSize);

			visible = true; // We just made them visible ^^
		}

		if (visible) {
			if (entity->IsPlayer()) {
				auto* player = reinterpret_cast<CBasePlayer*>(entity);
				if (!player->IsAlive() || *player->Team() == TeamID::TEAM_UNASSIGNED || entity == spectatorEntity)
					continue;
				PlayerStateSettings* settings;
				if (entity == GameCache::GetLocalPlayer()) // TODO Check for third person
					settings = &players.local;
				else if (!player->GetDormant() && (*player->Team() == TeamID::TEAM_SPECTATOR || !player->IsAlive())) {
					char name[128];
					if (players.spectators.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
						PlayerInfo info {};
						Interfaces::engine->GetPlayerInfo(player->entindex(), &info);
						strcpy(name, info.name);
					}
					players.spectators.Draw(drawList, rectangle, name);
					continue;
				} else {
					if (!player->IsEnemy(localPlayer))
						settings = SelectPlayerState(localPlayer, player, &players.teammate);
					else
						settings = SelectPlayerState(localPlayer, player, &players.enemy);
				}

				if (settings)
					settings->Draw(drawList, rectangle, player);
			} else if (entity->IsWeapon()) {
				auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(entity);
				if (*weapon->OwnerEntity() == -1)
					weapons.Draw(drawList, rectangle, weapon);
			} else {
				const ClientClass clientClass = *entity->GetClientClass();
				switch (static_cast<ClientClassID>(clientClass.m_ClassID)) {
				case ClientClassID::CPlantedC4: {
					auto* bomb = reinterpret_cast<CPlantedC4*>(entity);
					plantedC4.Draw(drawList, rectangle, bomb);
					break;
				}
				case ClientClassID::CBaseCSGrenadeProjectile:
					// TODO Seperate
					projectiles.Draw(drawList, rectangle, xorstr_("Base grenade"));
					break;
				case ClientClassID::CBreachChargeProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Breach charge"));
					break;
				case ClientClassID::CBumpMineProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Bump Mine"));
					break;
				case ClientClassID::CDecoyProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Decoy"));
					break;
				case ClientClassID::CMolotovProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Molotov"));
					break;
				case ClientClassID::CSensorGrenadeProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Sensor grenade"));
					break;
				case ClientClassID::CSmokeGrenadeProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Smoke grenade"));
					break;
				case ClientClassID::CSnowballProjectile:
					projectiles.Draw(drawList, rectangle, xorstr_("Snowball"));
					break;
				case ClientClassID::CHostage:
					hostages.Draw(drawList, rectangle, xorstr_("Hostage"));
					break;
				case ClientClassID::CPhysPropLootCrate:
					// TODO Separate
					dzLootCrates.Draw(drawList, rectangle, xorstr_("Loot crate"));
					break;
				case ClientClassID::CPhysPropAmmoBox:
					dzAmmoBoxes.Draw(drawList, rectangle, xorstr_("Ammo box"));
					break;
				case ClientClassID::CDronegun:
					dzSentries.Draw(drawList, rectangle, xorstr_("Sentry"));
					break;
				default:
					other.Draw(drawList, rectangle, clientClass.m_pNetworkName);
					break;
				}
			}
		}
	}
}

void Features::Visuals::Esp::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::SameLine();
	ImGui::SliderInt(xorstr_("Draw distance"), &drawDistance, 0, 1024 * 16);

	ImGui::Checkbox(xorstr_("Consider spotted entities as visible"), &considerSpottedEntitiesAsVisible);
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImGuiColors::red.Value);
	ImGui::Checkbox(xorstr_("Cache visibility state"), &cacheVisibilityState);
	ImGui::PopStyleColor();
	ImGui::HelpMarker(xorstr_("Debug option, which should only be turned off if needed"));

	ImGui::Checkbox(xorstr_("Consider smoked off entities as occluded"), &considerSmokedOffEntitiesAsOccluded);
	ImGui::SameLine();
	ImGui::Checkbox(xorstr_("Out of view"), &outOfView);
	ImGui::SameLine();

	if (ImGui::Popup(xorstr_("Out of view settings"))) {
		ImGui::SliderFloat(xorstr_("Size"), &outOfViewSize, 0.0f, 50.0f, xorstr_("%.2f"));
		ImGui::SliderFloat(xorstr_("Distance"), &outOfViewDistance, 0.0f, 500.0f, xorstr_("%.2f"));
		ImGui::EndPopup();
	}

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
SERIALIZED_TYPE(xorstr_("Cache visibility state"), cacheVisibilityState)

SERIALIZED_TYPE(xorstr_("Out of view"), outOfView)
SERIALIZED_TYPE(xorstr_("Out of view size"), outOfViewSize)
SERIALIZED_TYPE(xorstr_("Out of view distance"), outOfViewDistance)

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
