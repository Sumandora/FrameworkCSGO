#include "../../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../GameCache.hpp"
#include "../../../Interfaces.hpp"

#include "../../../Utils/Raytrace.hpp"

#include "../../../GUI/Elements/Keybind.hpp"

#include "../../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include "../../../SDK/ClientClassIDs.hpp"

#include <bits/stdc++.h>
#include <cstdint>
#include <vector>

bool Features::Visuals::Esp::enabled = false;
int Features::Visuals::Esp::onKey = 0;
int Features::Visuals::Esp::drawDistance = 1024 * 8;
bool Features::Visuals::Esp::considerSpottedEntitiesAsVisible = false;
bool Features::Visuals::Esp::considerSmokedOffEntitiesAsOccluded = true;
PlayerSettings Features::Visuals::Esp::players { strdup(xorstr_("Players")) };
WeaponSettings Features::Visuals::Esp::weapons { strdup(xorstr_("Weapons")) };
BoxNameSetting Features::Visuals::Esp::projectiles { strdup(xorstr_("Projectiles")) };
PlantedC4Settings Features::Visuals::Esp::plantedC4 { strdup(xorstr_("Planted C4")) };
BoxNameSetting Features::Visuals::Esp::hostages { strdup(xorstr_("Hostages")) };
BoxNameSetting Features::Visuals::Esp::dzLootCrates { strdup(xorstr_("Loot crates")) };
BoxNameSetting Features::Visuals::Esp::dzAmmoBoxes { strdup(xorstr_("Ammo boxes")) };
BoxNameSetting Features::Visuals::Esp::dzSentries { strdup(xorstr_("Sentries")) };
BoxNameSetting Features::Visuals::Esp::other { strdup(xorstr_("Other")) };

bool WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition)
{
	const float z = matrix[2][0] * worldPosition.x + matrix[2][1] * worldPosition.y + matrix[2][2] * worldPosition.z + matrix[2][3];
	const float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
	if (z <= 0.0f || w <= 0.0f)
		return false;

	screenPosition = ImVec2(ImGui::GetIO().DisplaySize);
	screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;

	screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
	screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
	return true;
}

PlayerStateSettings* SelectPlayerState(CBasePlayer* viewer, CBasePlayer* player, PlayerTeamSettings* settings)
{
	if (player->GetDormant())
		return &settings->dormant;

	if(settings->visible == settings->occluded)
		return &settings->visible; // Having visible == occluded is a common configuration, we can skip most of this function if it is the case

	if (settings == &Features::Visuals::Esp::players.enemy /* Teammates are always "spotted" */ && Features::Visuals::Esp::considerSpottedEntitiesAsVisible && *player->Spotted())
		return &settings->visible; // Don't even have to raytrace for that.

	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
	if (!player->SetupBones(boneMatrix))
		return &settings->dormant; // Setup bones is broken??

	const Vector head = boneMatrix[8].Origin();

	if (Features::Visuals::Esp::considerSmokedOffEntitiesAsOccluded && Memory::LineGoesThroughSmoke(viewer->GetEyePosition(), head, 1))
		return &settings->occluded;

	CTraceFilterEntity filter(viewer);
	const Trace trace = Utils::TraceRay(viewer->GetEyePosition(), head, &filter);

	if (trace.m_pEnt != player)
		return &settings->occluded;
	else
		return &settings->visible;
}

void Features::Visuals::Esp::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* viewer = GameCache::GetLocalPlayer();

	if (!viewer)
		return;

	if (*viewer->LifeState() == LIFE_DEAD && viewer->ObserverTarget()) {
		auto* observerTarget = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntityFromHandle(viewer->ObserverTarget()));
		if (observerTarget && *observerTarget->LifeState() == LIFE_ALIVE)
			viewer = observerTarget;
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

		if ((*entity->Origin() - *viewer->Origin()).LengthSquared() > (float)(drawDistance * drawDistance))
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

		if (visible) {
			if (entity->IsPlayer()) {
				auto* player = reinterpret_cast<CBasePlayer*>(entity);
				if (*player->LifeState() != LIFE_ALIVE || *player->Team() == TeamID::TEAM_UNASSIGNED)
					continue;
				PlayerStateSettings* settings = nullptr;
				if (entity == GameCache::GetLocalPlayer()) // TODO Check for third person
					settings = &players.local;
				else if (!player->GetDormant() && (*player->Team() == TeamID::TEAM_SPECTATOR || *player->LifeState() != LIFE_ALIVE)) {
					char name[128];
					if (players.spectators.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
						PlayerInfo info {};
						Interfaces::engine->GetPlayerInfo(player->entindex(), &info);
						strcpy(name, info.name);
					}
					players.spectators.Draw(drawList, rectangle, name);
					continue;
				} else {
					if (!player->IsEnemy())
						settings = SelectPlayerState(viewer, player, &players.teammate);
					else
						settings = SelectPlayerState(viewer, player, &players.enemy);
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
	ImGui::Checkbox(xorstr_("Consider smoked off entities as occluded"), &considerSmokedOffEntitiesAsOccluded);

	ImGui::InputSelector(xorstr_("Hold key (%s)"), onKey);

	if (ImGui::BeginTabBar(xorstr_("#Config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Players"))) {
			players.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Weapons"))) {
			weapons.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Projectiles"))) {
			projectiles.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Planted C4"))) {
			plantedC4.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Hostages"))) {
			hostages.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Danger Zone"))) {
			if (ImGui::BeginTabBar(xorstr_("#Danger Zone config selection"), ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem(xorstr_("Loot crates"))) {
					dzLootCrates.SetupGUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Ammo boxes"))) {
					dzAmmoBoxes.SetupGUI();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Sentries"))) {
					dzSentries.SetupGUI();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Other"))) {
			other.SetupGUI();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::Esp::Serializer, xorstr_("Esp"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Draw distance"), drawDistance)
SERIALIZED_TYPE(xorstr_("Hold key"), onKey)
SERIALIZED_TYPE(xorstr_("Consider spotted entities as visible"), considerSpottedEntitiesAsVisible)
SERIALIZED_TYPE(xorstr_("Consider smoked off entities as occluded"), considerSmokedOffEntitiesAsOccluded)

SERIALIZED_STRUCTURE(players)
SERIALIZED_STRUCTURE(weapons)
SERIALIZED_STRUCTURE(projectiles)
SERIALIZED_STRUCTURE(plantedC4)
SERIALIZED_STRUCTURE(hostages)
SERIALIZED_STRUCTURE(dzLootCrates)
SERIALIZED_STRUCTURE(dzAmmoBoxes)
SERIALIZED_STRUCTURE(dzSentries)
SERIALIZED_STRUCTURE(other)
END_SERIALIZED_STRUCT
