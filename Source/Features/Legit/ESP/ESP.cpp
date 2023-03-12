#include "ESP.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../GameCache.hpp"
#include "../../../Interfaces.hpp"

#include "../../../Utils/PlayerIds.hpp"
#include "../../../Utils/Raytrace.hpp"

#include "../../../GUI/Elements/Keybind.hpp"

#include "../../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include "../../../SDK/ClientClassIDs.hpp"
#include "../../../SDK/Definitions/LifeState.hpp"

#include "../../../Memory.hpp"

#include <bits/stdc++.h>
#include <cstdint>
#include <vector>

bool Features::Legit::Esp::enabled = false;
int Features::Legit::Esp::onKey = 0;
int Features::Legit::Esp::drawDistance = 1024 * 8;
bool Features::Legit::Esp::considerSpottedEntitiesAsVisible = false;
bool Features::Legit::Esp::considerSmokedOffEntitiesAsOccluded = true;
PlayerSettings Features::Legit::Esp::players { strdup(xorstr_("Players")) };
WeaponSettings Features::Legit::Esp::weapons { strdup(xorstr_("Weapons")) };
BoxNameSetting Features::Legit::Esp::projectiles { strdup(xorstr_("Projectiles")) };
PlantedC4Settings Features::Legit::Esp::plantedC4 { strdup(xorstr_("Planted C4")) };
BoxNameSetting Features::Legit::Esp::dzLootCrates { strdup(xorstr_("Loot crates")) };
BoxNameSetting Features::Legit::Esp::dzAmmoBoxes { strdup(xorstr_("Ammo boxes")) };
BoxNameSetting Features::Legit::Esp::dzSentries { strdup(xorstr_("Sentries")) };

bool WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition)
{
	float z = matrix[2][0] * worldPosition.x + matrix[2][1] * worldPosition.y + matrix[2][2] * worldPosition.z + matrix[2][3];
	float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
	if (z <= 0.0f || w <= 0.0f)
		return false;

	screenPosition = ImVec2(ImGui::GetIO().DisplaySize);
	screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;

	screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
	screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
	return true;
}

void DrawPlayer(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player, PlayerStateSettings* settings)
{
	char name[128];
	if (settings->boxName.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		int index = Utils::GetEntityId(player);

		PlayerInfo info {};
		Interfaces::engine->GetPlayerInfo(index, &info);
		strcpy(name, info.name);
	}
	settings->boxName.Draw(drawList, rectangle, name);

	settings->healthbar.Draw(drawList, rectangle, std::clamp(*player->Health() / 100.0f, 0.0f, 1.0f));
	if (settings->weapon.enabled) { // Don't ask for the weapon, if we don't have to
		CBaseCombatWeapon* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(player->ActiveWeapon()));
		if (weapon) {
			WeaponID weaponID = *weapon->WeaponDefinitionIndex();
			if (weaponID > WeaponID::WEAPON_NONE) { // Also prevent invalids
				char weaponName[256];
				LocalizeWeaponID(weaponID, weaponName);
				settings->weapon.Draw(drawList, rectangle, weaponName, 1.0f);
			}
		}
	}

	float flashDuration = *reinterpret_cast<float*>(reinterpret_cast<char*>(player->FlashMaxAlpha()) - 0x8);
	if (flashDuration > 0.0) {
		settings->flashDuration.Draw(drawList, rectangle, std::to_string(static_cast<int>(flashDuration)).c_str(), 0.5f);
	}
}

PlayerStateSettings* SelectPlayerState(CBasePlayer* player, PlayerTeamSettings* settings)
{
	if (player->GetDormant())
		return &settings->dormant;

	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
	if (!player->SetupBones(boneMatrix))
		return &settings->dormant; // Setup bones is broken??

	if (settings == &Features::Legit::Esp::players.enemy /* Teammates are always "spotted" */ && Features::Legit::Esp::considerSpottedEntitiesAsVisible && *player->Spotted())
		return &settings->visible; // Don't even have to raytrace for that.

	Vector head = boneMatrix[8].Origin();

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (Features::Legit::Esp::considerSmokedOffEntitiesAsOccluded && Memory::LineGoesThroughSmoke(localPlayer->GetEyePosition(), head, 1))
		return &settings->occluded;

	CTraceFilterEntity filter(localPlayer);
	Trace trace = Utils::TraceRay(localPlayer->GetEyePosition(), head, &filter);

	if (trace.m_pEnt != player)
		return &settings->occluded;
	else
		return &settings->visible;
}

void Features::Legit::Esp::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !IsInputDown(onKey, true))
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (!Interfaces::engine->IsInGame() || localPlayer == nullptr)
		return;

	Matrix4x4 matrix = Hooks::FrameStageNotify::worldToScreenMatrix;

	if (!matrix.Base())
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::entityList->GetHighestEntityIndex(); i++) {
		auto entity = reinterpret_cast<CBaseEntity*>(Interfaces::entityList->GetClientEntity(i));
		if (!entity)
			continue;
		if ((*entity->VecOrigin() - *localPlayer->VecOrigin()).LengthSquared() > drawDistance * drawDistance)
			continue;

		CCollideable* collideable = entity->Collision();

		Vector min = *entity->VecOrigin() + *collideable->ObbMins();
		Vector max = *entity->VecOrigin() + *collideable->ObbMaxs();

		Vector points[] = {
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
				auto player = reinterpret_cast<CBasePlayer*>(entity);
				PlayerStateSettings* settings = nullptr;
				if (entity == GameCache::GetLocalPlayer()) // TODO Check for third person
					settings = &players.local;
				else if (!player->GetDormant() && (*player->Team() == TeamID::TEAM_SPECTATOR || *player->LifeState() != LIFE_ALIVE)) {
					char name[128];
					if (players.spectators.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
						int index = Utils::GetEntityId(player);

						PlayerInfo info {};
						Interfaces::engine->GetPlayerInfo(index, &info);
						strcpy(name, info.name);
					}
					players.spectators.Draw(drawList, rectangle, name);
					continue;
				} else if (*player->LifeState() == LIFE_ALIVE) {
					if (!player->IsEnemy())
						settings = SelectPlayerState(player, &players.teammate);
					else
						settings = SelectPlayerState(player, &players.enemy);
				}

				if (settings)
					DrawPlayer(drawList, rectangle, player, settings);
			} else if (entity->IsWeapon()) {
				auto weapon = reinterpret_cast<CBaseCombatWeapon*>(entity);
				if (*weapon->OwnerEntity() == -1)
					weapons.Draw(drawList, rectangle, weapon);
			} else {
				ClientClass clientClass = *entity->GetClientClass();
				switch (static_cast<ClientClassID>(clientClass.m_ClassID)) {
				case ClientClassID::CPlantedC4: {
					auto bomb = reinterpret_cast<CPlantedC4*>(entity);
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
					// TODO?
					break;
				}
			}
		}
	}
}

void Features::Legit::Esp::SetupGUI()
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

		ImGui::EndTabBar();
	}
}

BEGIN_SERIALIZED_STRUCT(Features::Legit::Esp::Serializer, xorstr_("Esp"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Draw distance"), drawDistance)
SERIALIZED_TYPE(xorstr_("Hold key"), onKey)
SERIALIZED_TYPE(xorstr_("Consider spotted entities as visible"), considerSpottedEntitiesAsVisible)
SERIALIZED_TYPE(xorstr_("Consider smoked off entities as occluded"), considerSmokedOffEntitiesAsOccluded)

SERIALIZED_STRUCTURE(players)
SERIALIZED_STRUCTURE(weapons)
SERIALIZED_STRUCTURE(projectiles)
SERIALIZED_STRUCTURE(plantedC4)
SERIALIZED_STRUCTURE(dzLootCrates)
SERIALIZED_STRUCTURE(dzAmmoBoxes)
SERIALIZED_STRUCTURE(dzSentries)
END_SERIALIZED_STRUCT
