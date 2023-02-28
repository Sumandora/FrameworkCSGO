#include "SpectatorList.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Interfaces.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include <map>
#include <vector>

bool Features::Legit::SpectatorList::enabled = false;

int	 getEntityId(CBaseEntity* entity) {
	 for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		 auto player2 = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		 if (player2 == entity) {
			 return i;
		 }
	 }
}

void mapObservers(std::map<int, int>& map) {
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player)
			continue;

		if (player->GetDormant() || player->LifeState() == LIFE_ALIVE)
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(player->ObserverTarget());
		if (!target)
			continue;

		map.emplace(i, getEntityId(target));
	}
}

const char* localizeObserverMode(ObserverMode observerMode) {
	switch (observerMode) {
	case ObserverMode::OBS_MODE_NONE:
		return "None";
	case ObserverMode::OBS_MODE_DEATHCAM:
		return "Deathcam";
	case ObserverMode::OBS_MODE_FREEZECAM:
		return "Freezecam";
	case ObserverMode::OBS_MODE_FIXED:
		return "Fixed";
	case ObserverMode::OBS_MODE_IN_EYE:
		return "In Eye";
	case ObserverMode::OBS_MODE_CHASE:
		return "Chase";
	case ObserverMode::OBS_MODE_ROAMING:
		return "Roaming";
	}
}

void Features::Legit::SpectatorList::ImGuiRender(ImDrawList* drawList) {
	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	std::map<int, int> map;
	mapObservers(map);

	CBaseEntity* currentTarget;
	int			 localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	auto localPlayer	  = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	if (localPlayer) {
		if (*localPlayer->LifeState() == LIFE_ALIVE)
			currentTarget = localPlayer;
		else {
			void* observerTarget = localPlayer->ObserverTarget();
			if (observerTarget)
				currentTarget = Interfaces::entityList->GetClientEntityFromHandle(observerTarget);
			else
				return;
		}
	} else
		return;

	int		playerTarget = getEntityId(currentTarget);

	ImVec2	displaySize	 = ImGui::GetIO().DisplaySize;
	float	offset		 = 0;

	ImColor white		 = ImColor(255, 255, 255, 255);
	ImColor red			 = ImColor(255, 0, 0, 255);
	ImColor black		 = ImColor(0, 0, 0, 255);

	for (auto entry : map) {
		PlayerInfo first {};
		Interfaces::engine->GetPlayerInfo(entry.first, &first);

		PlayerInfo second {};
		Interfaces::engine->GetPlayerInfo(entry.second, &second);

		ObserverMode observerMode = *reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(entry.first))->ObserverMode();

		char		 text[strlen(first.name) + 4 + strlen(second.name) + 1];
		sprintf(text, xorstr_("%s -> %s (%s)"), first.name, second.name, localizeObserverMode(observerMode));

		ImVec2 size = ImGui::CalcTextSize(text);
		ImVec2 position(displaySize.x - size.x - 10.0f, offset + 10.0f);

		drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), black, text);
		drawList->AddText(position, entry.second == playerTarget ? red : white, text);

		offset += ImGui::GetTextLineHeightWithSpacing();
	}
}

void Features::Legit::SpectatorList::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitSpectatorList"), &enabled);
}
