#include "ESP.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../../GameCache.hpp"
#include "../../../Interfaces.hpp"

#include "../../../Utils/PlayerIds.hpp"
#include "../../../Utils/Raytrace.hpp"

#include "../../../GUI/ImGuiColors.hpp"
#include "../../../GUI/Elements/Keybind.hpp"
#include "../../../GUI/Elements/ClickableColorButton.hpp"

#include "../../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include "../../../SDK/GameClass/CBaseAttributableItem.hpp"
#include "../../../SDK/GameClass/CPlantedC4.hpp"
#include "../../../SDK/ClientClassIDs.hpp"
#include "../../../SDK/Definitions/LifeState.hpp"

#include <vector>
#include <bits/stdc++.h>
#include <cstdint>

bool 				Features::Legit::Esp::enabled			= false;
int 				Features::Legit::Esp::onKey				= 0;
int 				Features::Legit::Esp::drawDistance		= 1024 * 32;
PlayerSettings		Features::Legit::Esp::players			{};
WeaponSettings		Features::Legit::Esp::weapons			{};
BoxNameSetting		Features::Legit::Esp::projectiles		{};
PlantedC4Settings	Features::Legit::Esp::plantedC4			{};
BoxNameSetting		Features::Legit::Esp::dzLootCrates		{};
BoxNameSetting		Features::Legit::Esp::dzAmmoBoxes		{};
BoxNameSetting		Features::Legit::Esp::dzSentries		{};

bool WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition) {
	float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
	if (w < 0.01f)
		return false;

	screenPosition	 = ImVec2(ImGui::GetIO().DisplaySize);
	screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;

	screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
	screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
	return true;
}

void DrawBox(ImDrawList* drawList, ImVec4 rectangle, BoxSettings settings) {
	if(!settings.enabled)
		return;
	if(settings.fill)
		drawList->AddRectFilled(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), settings.fillColor, settings.rounding, ImDrawFlags_None);
	if(settings.outlined)
		drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), settings.outlineColor, settings.rounding, ImDrawFlags_None, settings.thickness + settings.outlineThickness);
	drawList->AddRect(ImVec2(rectangle.x, rectangle.y), ImVec2(rectangle.z, rectangle.w), settings.color, settings.rounding, ImDrawFlags_None, settings.thickness);
}

void DrawTextSetting(ImDrawList* drawList, ImVec4 rectangle, const char* text, float height, TextSetting settings) {
	if(!settings.enabled)
		return;
	
	// Hack
	float fontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale = settings.fontScale;
	ImGui::PushFont(ImGui::GetFont());

	ImVec2 size = ImGui::CalcTextSize(text);

	float above = rectangle.y - size.y;
	float below = rectangle.w;
	ImVec2 position(rectangle.x + (rectangle.z - rectangle.x) * 0.5f - size.x / 2.0f, above + (below - above) * height);
	
	if(settings.shadow)
		drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), settings.shadowColor, text);

	drawList->AddText(position, settings.fontColor, text);
	
	ImGui::PopFont();
	ImGui::GetFont()->Scale = fontScale;
}

void DrawHealthbar(ImDrawList* drawList, ImVec4 rectangle, float health, HealthbarSettings& settings) {
	if(!settings.enabled)
		return;

	ImVec4 healthbar(rectangle.x - settings.spacing - settings.width, rectangle.y, rectangle.x - settings.spacing, rectangle.w);
	ImVec4 outside(healthbar.x - settings.outlineThickness, healthbar.y - settings.outlineThickness, healthbar.z + settings.outlineThickness, healthbar.w + settings.outlineThickness);

	ImVec4 background;

	if(settings.outlined) // If we are outlining it, we don't want a gap between the outline and the healthbar
		background = outside;
	else
		background = healthbar;

	drawList->AddRectFilled(ImVec2(background.x, background.y), ImVec2(background.z, background.w), settings.backgroundColor, settings.rounding);
	if(settings.outlined) {
		drawList->AddRect(ImVec2(outside.x, outside.y), ImVec2(outside.z, outside.w), settings.outlineColor, settings.rounding, ImDrawFlags_None, settings.outlineThickness);
	}

	float aliveHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		settings.aliveColor.Value.x, settings.aliveColor.Value.y, settings.aliveColor.Value.z,
		aliveHsv[0], aliveHsv[1], aliveHsv[2]
	);

	float deadHsv[3];
	ImGui::ColorConvertRGBtoHSV(
		settings.deadColor.Value.x, settings.deadColor.Value.y, settings.deadColor.Value.z,
		deadHsv[0], deadHsv[1], deadHsv[2]
	);

	float desiredHsv[] = {
		deadHsv[0] + (aliveHsv[0] - deadHsv[0]) * health,
		deadHsv[1] + (aliveHsv[1] - deadHsv[1]) * health,
		deadHsv[2] + (aliveHsv[2] - deadHsv[2]) * health
	};
	float alpha = settings.deadColor.Value.w + (settings.aliveColor.Value.w - settings.deadColor.Value.w) * health;

	float desiredRgb[3];
	ImGui::ColorConvertHSVtoRGB(
		desiredHsv[0], desiredHsv[1], desiredHsv[2],
		desiredRgb[0], desiredRgb[1], desiredRgb[2]
	);

	ImColor color(desiredRgb[0], desiredRgb[1], desiredRgb[2], alpha);
	drawList->AddRectFilled(ImVec2(healthbar.x, healthbar.y + (healthbar.w - healthbar.y) * (1.0 - health)), ImVec2(healthbar.z, healthbar.w), color, settings.rounding);
}

void DrawBoxName(ImDrawList* drawList, ImVec4 rectangle, const char* text, BoxNameSetting& settings) {
	DrawBox(drawList, rectangle, settings.box);
	DrawTextSetting(drawList, rectangle, text, 0.0f, settings.nametag);
}

void DrawPlayerWithName(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player, BoxNameSetting& settings) {
	char name[128];
	if(settings.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		int index = Utils::GetEntityId(player);

		PlayerInfo info {};
		Interfaces::engine->GetPlayerInfo(index, &info);
		strcpy(name, info.name);
	}
	DrawBoxName(drawList, rectangle, name, settings);
}

void DrawPlayer(ImDrawList* drawList, ImVec4 rectangle, CBasePlayer* player, PlayerStateSettings& settings) {
	DrawPlayerWithName(drawList, rectangle, player, settings);

	DrawHealthbar(drawList, rectangle, std::clamp(*player->Health() / 100.0f, 0.0f, 1.0f), settings.healthbar);
	if(settings.weapon.enabled) { // Don't ask for the weapon, if we don't have to
		CBaseCombatWeapon* weapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(player->ActiveWeapon()));
		if(weapon) {
			WeaponID weaponID = *weapon->WeaponDefinitionIndex();
			if(weaponID > WeaponID::WEAPON_NONE) { // Also prevent invalids
				char weaponName[256];
				LocalizeWeaponID(weaponID, weaponName);
				DrawTextSetting(drawList, rectangle, weaponName, 1.0f, settings.weapon);
			}
		}
	}

	float flashDuration = *reinterpret_cast<float*>(reinterpret_cast<char*>(player->FlashMaxAlpha()) - 0x8);
	if(flashDuration > 0.0) {
		DrawTextSetting(drawList, rectangle, std::to_string(static_cast<int>(flashDuration)).c_str(), 0.5f, settings.flashDuration);
	}
}

void DrawWeapon(ImDrawList* drawList, ImVec4 rectangle, CBaseCombatWeapon* weapon, WeaponSettings& settings) {
	char weaponName[256]{};
	if(settings.nametag.enabled) { // Don't ask for the weapon, if we don't have to
		LocalizeWeaponID(*weapon->WeaponDefinitionIndex(), weaponName);
	}
	DrawBoxName(drawList, rectangle, weaponName, settings);

	DrawTextSetting(drawList, rectangle, std::to_string(*weapon->Ammo()).c_str(), 1.0f, settings.ammo);
}

void DrawPlantedC4(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb, PlantedC4Settings& settings) {
	DrawBoxName(drawList, rectangle, xorstr_("Planted C4"), settings);
	DrawTextSetting(drawList, rectangle, std::to_string(*bomb->BombTime() - Memory::globalVars->curtime).c_str(), 1.0f, settings.timer);
}

PlayerStateSettings SelectPlayerState(CBasePlayer* player, PlayerTeamSettings settings) {
	if(player->GetDormant())
		return settings.dormant;

	Matrix3x4 boneMatrix[MAXSTUDIOBONES];
	if(!player->SetupBones(boneMatrix))
		return settings.dormant; // Setup bones is broken??

	Vector head = boneMatrix[8].Origin();

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	CTraceFilterEntity filter(localPlayer);
	Trace trace = Utils::TraceRay(localPlayer->GetEyePosition(), head, &filter);

	if(trace.m_pEnt != player)
		return settings.occluded;
	else
		return settings.visible;
}

void Features::Legit::Esp::ImGuiRender(ImDrawList* drawList) {
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
		if((*entity->VecOrigin() - *localPlayer->VecOrigin()).LengthSquared() > drawDistance * drawDistance)
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

		auto   topLeft = ImVec2(ImGui::GetIO().DisplaySize); // hacky but hey, it works
		ImVec2 bottomRight;
		bool   visible = true;

		for (const auto& point : points) {
			ImVec2 point2D;

			if (!WorldToScreen(matrix, point, point2D)) {
				visible = false;
				break;
			}

			if (point2D.x < topLeft.x)
				topLeft.x = point2D.x;

			if (point2D.y < topLeft.y)
				topLeft.y = point2D.y;

			if (point2D.x > bottomRight.x)
				bottomRight.x = point2D.x;

			if (point2D.y > bottomRight.y)
				bottomRight.y = point2D.y;
		}

		if (visible) {
			ImVec4 rectangle(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
			if(entity->IsPlayer()) {
				auto player = reinterpret_cast<CBasePlayer*>(entity);
				PlayerStateSettings settings;
				if(entity == GameCache::GetLocalPlayer()) // TODO Check for third person
					settings = players.local;
				else if(!player->GetDormant() && (*player->Team() == TeamID::TEAM_SPECTATOR || *player->LifeState() != LIFE_ALIVE)) {
					DrawPlayerWithName(drawList, rectangle, player, players.spectators);
					continue;
				}
				else if(*player->LifeState() == LIFE_ALIVE) {
					if(!player->IsEnemy())
						settings = SelectPlayerState(player, players.teammate);
					else
						settings = SelectPlayerState(player, players.enemy);
				}

				DrawPlayer(drawList, rectangle, player, settings);
			} else if(entity->IsWeapon()) {
				auto weapon = reinterpret_cast<CBaseCombatWeapon*>(entity);
				if(*weapon->OwnerEntity() == -1)
					DrawWeapon(drawList, rectangle, weapon, weapons);
			} else {
				ClientClass clientClass = *entity->GetClientClass();
				switch(static_cast<ClientClassID>(clientClass.m_ClassID)) {
				case ClientClassID::CPlantedC4: {
					auto bomb = reinterpret_cast<CPlantedC4*>(entity);
					DrawPlantedC4(drawList, rectangle, bomb, plantedC4);
					break;
				}
				case ClientClassID::CBaseCSGrenadeProjectile:
					//TODO Seperate
					DrawBoxName(drawList, rectangle, xorstr_("Base grenade"), projectiles);
					break;
				case ClientClassID::CBreachChargeProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Breach charge"), projectiles);
					break;
				case ClientClassID::CBumpMineProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Bump Mine"), projectiles);
					break;
				case ClientClassID::CDecoyProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Decoy"), projectiles);
					break;
				case ClientClassID::CMolotovProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Molotov"), projectiles);
					break;
				case ClientClassID::CSensorGrenadeProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Sensor grenade"), projectiles);
					break;
				case ClientClassID::CSmokeGrenadeProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Smoke grenade"), projectiles);
					break;
				case ClientClassID::CSnowballProjectile:
					DrawBoxName(drawList, rectangle, xorstr_("Snowball"), projectiles);
					break;
				case ClientClassID::CPhysPropLootCrate:
					DrawBoxName(drawList, rectangle, xorstr_("Loot crate"), dzLootCrates);
					break;
				case ClientClassID::CPhysPropAmmoBox:
					DrawBoxName(drawList, rectangle, xorstr_("Ammo box"), dzAmmoBoxes);
					break;
				case ClientClassID::CDronegun:
					DrawBoxName(drawList, rectangle, xorstr_("Sentry"), dzSentries);
					break;
				default:
					//TODO?
					break;
				}
			}

		}
	}
}

// Yes the "..."-Popups are inspired by danielkrupinski/Osiris

void ShowBoxSettings(const char* tag, BoxSettings& boxSettings) {
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &boxSettings.enabled);

	ImGui::SameLine();
	if(ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if(ImGui::BeginPopup(tag)) {
		ImGui::ClickableColorButton(xorstr_("Color"), boxSettings.color);
		ImGui::SliderFloat(xorstr_("Rounding"), &boxSettings.rounding, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Thickness"), &boxSettings.thickness, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox(xorstr_("Outlined"), &boxSettings.outlined);
		if(boxSettings.outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), boxSettings.outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &boxSettings.outlineThickness, 0.0f, 10.0f, "%.2f");
		}
		ImGui::Checkbox(xorstr_("Fill"), &boxSettings.fill);
		if(boxSettings.fill)
			ImGui::ClickableColorButton(xorstr_("Fill color"), boxSettings.fillColor);
		
		ImGui::EndPopup();
	}

	ImGui::PopID();
}

void ShowHealthbarSettings(const char* tag, HealthbarSettings& healthbarSettings) {
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &healthbarSettings.enabled);

	ImGui::SameLine();
	if(ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if(ImGui::BeginPopup(tag)) {
		ImGui::ClickableColorButton(xorstr_("Background color"), healthbarSettings.backgroundColor);
		ImGui::SliderFloat(xorstr_("Rounding"), &healthbarSettings.rounding, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Spacing"), &healthbarSettings.spacing, 0.0f, 10.0f, "%.2f");
		ImGui::SliderFloat(xorstr_("Width"), &healthbarSettings.width, 0.0f, 10.0f, "%.2f");
		
		ImGui::ClickableColorButton(xorstr_("Alive color"), healthbarSettings.aliveColor);
		ImGui::ClickableColorButton(xorstr_("Dead color"), healthbarSettings.deadColor);
		
		ImGui::Checkbox(xorstr_("Outlined"), &healthbarSettings.outlined);
		if(healthbarSettings.outlined) {
			ImGui::ClickableColorButton(xorstr_("Outline color"), healthbarSettings.outlineColor);
			ImGui::SliderFloat(xorstr_("Outline thickness"), &healthbarSettings.outlineThickness, 0.0f, 10.0f, "%.2f");
		}
		
		ImGui::EndPopup();
	}

	ImGui::PopID();
}

void ShowTextSetting(const char* tag, TextSetting& textSetting) {
	ImGui::PushID(tag);
	ImGui::Checkbox(tag, &textSetting.enabled);

	ImGui::SameLine();
	if(ImGui::Button("..."))
		ImGui::OpenPopup(tag);

	if(ImGui::BeginPopup(tag)) {
		ImGui::SliderFloat(xorstr_("Font scale"), &textSetting.fontScale, 0.1f, 2.0f, "%.2f");
		ImGui::ClickableColorButton(xorstr_("Font color"), textSetting.fontColor);
		ImGui::Checkbox(xorstr_("Shadow"), &textSetting.shadow);
		if(textSetting.shadow)
			ImGui::ClickableColorButton(xorstr_("Shadow color"), textSetting.shadowColor);
		
		ImGui::EndPopup();
	}

	ImGui::PopID();
}

void ShowBoxNameSetting(const char* tag, BoxNameSetting& boxNameSetting) {
	ImGui::PushID(tag);
	ShowBoxSettings(xorstr_("Box"), boxNameSetting.box);
	ShowTextSetting(xorstr_("Name"), boxNameSetting.nametag);
	ImGui::PopID();
}

void CopyPlayerStateSettings(PlayerStateSettings from, PlayerStateSettings& to) {
	to.box = from.box;
	to.nametag = from.nametag;
	to.healthbar = from.healthbar;
	to.weapon = from.weapon;
	to.flashDuration = from.flashDuration;
}

void BuildMenu(PlayerStateSettings& playerStateSettings, PlayerTeamSettings playerTeamSettings) {
	if(ImGui::MenuItem(xorstr_("Visible"))) {
		CopyPlayerStateSettings(playerTeamSettings.visible, playerStateSettings);
	}
	if(ImGui::MenuItem(xorstr_("Occluded"))) {
		CopyPlayerStateSettings(playerTeamSettings.occluded, playerStateSettings);
	}
	if(ImGui::MenuItem(xorstr_("Dormant"))) {
		CopyPlayerStateSettings(playerTeamSettings.dormant, playerStateSettings);
	}
}

void ShowPlayerStateSettings(const char* tag, PlayerStateSettings& playerStateSettings) {
	ImGui::PushID(tag);
	if(ImGui::Button(xorstr_("Copy from")))
		ImGui::OpenPopup(xorstr_("##Copy from"));

	if(ImGui::BeginPopup(xorstr_("##Copy from"))) {
		if(ImGui::BeginMenu(xorstr_("Enemy"))) {
			BuildMenu(playerStateSettings, Features::Legit::Esp::players.enemy);
			ImGui::EndMenu();
		}
		if(ImGui::Selectable(xorstr_("Teammate"))) {
			BuildMenu(playerStateSettings, Features::Legit::Esp::players.teammate);
			ImGui::EndMenu();
		}
		if(ImGui::Selectable(xorstr_("Local"))) {
			CopyPlayerStateSettings(Features::Legit::Esp::players.local, playerStateSettings);
		}
		ImGui::EndPopup();
	}

	ShowBoxNameSetting(tag, playerStateSettings);
	ShowHealthbarSettings(xorstr_("Healthbar"), playerStateSettings.healthbar);
	ShowTextSetting(xorstr_("Weapon"), playerStateSettings.weapon);
	ShowTextSetting(xorstr_("Flash duration"), playerStateSettings.flashDuration);
	ImGui::PopID();
}

void ShowPlayerTeamSettings(const char* tag, PlayerTeamSettings& playerTeamSettings) {
	ImGui::PushID(tag);
	if (ImGui::BeginTabBar(xorstr_("#Player team config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Visible"))) {
			ShowPlayerStateSettings(xorstr_("Visible"), playerTeamSettings.visible);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Occluded"))) {
			ShowPlayerStateSettings(xorstr_("Occluded"), playerTeamSettings.occluded);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Dormant"))) {
			ShowPlayerStateSettings(xorstr_("Dormant"), playerTeamSettings.dormant);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

void ShowPlayerSettings(const char* tag, PlayerSettings& playerSettings) {
	ImGui::PushID(tag);
	if (ImGui::BeginTabBar(xorstr_("#Player config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Enemy"))) {
			ShowPlayerTeamSettings(xorstr_("Enemy"), playerSettings.enemy);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Teammate"))) {
			ShowPlayerTeamSettings(xorstr_("Teammate"), playerSettings.teammate);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Local"))) {
			ShowPlayerStateSettings(xorstr_("Local"), playerSettings.local);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Spectators"))) {
			ShowBoxNameSetting(xorstr_("Spectators"), playerSettings.spectators);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::PopID();
}

void ShowWeaponSettings(const char* tag, WeaponSettings& weaponSettings) {
	ImGui::PushID(tag);
	ShowBoxNameSetting(tag, weaponSettings);
	ShowTextSetting(xorstr_("Ammo"), weaponSettings.ammo);
	ImGui::PopID();
}

void ShowPlantedC4Settings(const char* tag, PlantedC4Settings& plantedC4Settings) {
	ImGui::PushID(tag);
	ShowBoxNameSetting(tag, plantedC4Settings);
	ShowTextSetting(xorstr_("Timer"), plantedC4Settings.timer);
	ImGui::PopID();
}

void Features::Legit::Esp::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitESP"), &enabled);
	ImGui::SameLine();
	ImGui::SliderInt(xorstr_("Draw distance"), &drawDistance, 0, 1024 * 64);
	ImGui::InputSelector(xorstr_("Hold key (%s)##LegitESP"), onKey);

	if (ImGui::BeginTabBar(xorstr_("#Config selection"), ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem(xorstr_("Players"))) {
			ShowPlayerSettings(xorstr_("Players"), players);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Weapons"))) {
			ShowWeaponSettings(xorstr_("Weapons"), weapons);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Projectiles"))) {
			ShowBoxNameSetting(xorstr_("Projectiles"), projectiles);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Planted C4"))) {
			ShowPlantedC4Settings(xorstr_("Planted C4"), plantedC4);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(xorstr_("Danger Zone"))) {
			if (ImGui::BeginTabBar(xorstr_("#Danger Zone config selection"), ImGuiTabBarFlags_Reorderable)) {
				if (ImGui::BeginTabItem(xorstr_("Loot crates"))) {
					ShowBoxNameSetting(xorstr_("Loot crates"), dzLootCrates);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Ammo boxes"))) {
					ShowBoxNameSetting(xorstr_("Ammo boxes"), dzAmmoBoxes);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem(xorstr_("Sentries"))) {
					ShowBoxNameSetting(xorstr_("Sentries"), dzSentries);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}
