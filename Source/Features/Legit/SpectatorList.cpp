#include "../Legit.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"

#include <vector>

bool Features::Legit::SpectatorList::enabled = false;

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

	auto* localPlayer = GameCache::GetLocalPlayer();

	CBaseEntity* currentTarget = nullptr;
	if (localPlayer) {
		if (*localPlayer->LifeState() == LIFE_ALIVE)
			currentTarget = localPlayer;
		else {
			// Intentionally not dereferenced, because game is weird
			void* observerTarget = localPlayer->ObserverTarget();
			if (observerTarget)
				currentTarget = Interfaces::entityList->GetClientEntityFromHandle(observerTarget);
		}
	}

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	float offset = 0;

	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player)
			continue;

		if (player->GetDormant() || *player->LifeState() != LIFE_DEAD)
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(player->ObserverTarget());
		if (!target)
			continue;

		PlayerInfo first {};
		Interfaces::engine->GetPlayerInfo(i, &first);

		PlayerInfo second {};
		Interfaces::engine->GetPlayerInfo(target->entindex(), &second);

		const ObserverMode observerMode = *player->ObserverMode();

		char text[strlen(first.name) + 4 + strlen(second.name) + 1];
		sprintf(text, xorstr_("%s -> %s (%s)"), first.name, second.name, LocalizeObserverMode(observerMode));

		const ImVec2 size = ImGui::CalcTextSize(text);
		const ImVec2 position(displaySize.x - size.x - 10.0F, offset + 10.0F);

		ShadowString::AddText(drawList, position, currentTarget && currentTarget == target ? ImGuiColors::red : ImGuiColors::white, text);

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
