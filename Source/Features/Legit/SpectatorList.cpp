#include "SpectatorList.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Utils/PlayerIds.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include <map>
#include <vector>

bool Features::Legit::SpectatorList::enabled = false;

void MapObservers(std::map<int, int>& map)
{
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player)
			continue;

		if (player->GetDormant() || *player->LifeState() != LIFE_DEAD)
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(player->ObserverTarget());
		if (!target)
			continue;

		map.emplace(i, Utils::GetEntityId(target));
	}
}

const char* LocalizeObserverMode(ObserverMode observerMode)
{
	switch (observerMode) {
	case ObserverMode::OBS_MODE_NONE:
		return xorstr_("None");
	case ObserverMode::OBS_MODE_DEATHCAM:
		return xorstr_("Deathcam");
	case ObserverMode::OBS_MODE_FREEZECAM:
		return xorstr_("Freezecam");
	case ObserverMode::OBS_MODE_FIXED:
		return xorstr_("Fixed");
	case ObserverMode::OBS_MODE_IN_EYE:
		return xorstr_("In Eye");
	case ObserverMode::OBS_MODE_CHASE:
		return xorstr_("Chase");
	case ObserverMode::OBS_MODE_ROAMING:
		return xorstr_("Roaming");
	}
}

void Features::Legit::SpectatorList::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	std::map<int, int> map;
	MapObservers(map);

	auto localPlayer = GameCache::GetLocalPlayer();

	int targetIndex;

	if (localPlayer) {
		CBaseEntity* currentTarget;
		if (*localPlayer->LifeState() == LIFE_ALIVE)
			currentTarget = localPlayer;
		else {
			// Intentionally not dereferenced, because game is weird
			void* observerTarget = localPlayer->ObserverTarget();
			if (observerTarget)
				currentTarget = Interfaces::entityList->GetClientEntityFromHandle(observerTarget);
		}
		targetIndex = Utils::GetEntityId(currentTarget);
	} else
		targetIndex = -1;

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	float offset = 0;

	for (auto entry : map) {
		PlayerInfo first {};
		Interfaces::engine->GetPlayerInfo(entry.first, &first);

		PlayerInfo second {};
		Interfaces::engine->GetPlayerInfo(entry.second, &second);

		CBaseEntity* otherEntity = Interfaces::entityList->GetClientEntity(entry.first);
		if (!otherEntity)
			continue;

		ObserverMode observerMode = *reinterpret_cast<CBasePlayer*>(otherEntity)->ObserverMode();

		char text[strlen(first.name) + 4 + strlen(second.name) + 1];
		sprintf(text, xorstr_("%s -> %s (%s)"), first.name, second.name, LocalizeObserverMode(observerMode));

		ImVec2 size = ImGui::CalcTextSize(text);
		ImVec2 position(displaySize.x - size.x - 10.0f, offset + 10.0f);

		printf("%d == %d", targetIndex, entry.second);
		printf("%f|%f|%f|%f", ImGuiColors::red.Value.x, ImGuiColors::red.Value.y, ImGuiColors::red.Value.z, ImGuiColors::red.Value.w);
		ShadowString::AddText(drawList, position, targetIndex != -1 && entry.second == targetIndex ? ImGuiColors::red : ImGuiColors::white, text);

		offset += ImGui::GetTextLineHeightWithSpacing();
	}
}

void Features::Legit::SpectatorList::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
}

BEGIN_SERIALIZED_STRUCT(Features::Legit::SpectatorList::Serializer, xorstr_("Spectator list"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
END_SERIALIZED_STRUCT
