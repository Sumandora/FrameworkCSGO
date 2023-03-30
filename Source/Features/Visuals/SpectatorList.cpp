#include "../Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"

#include "../../Interfaces.hpp"

#include <vector>

static bool enabled = false;

void LocalizeObserverMode(ObserverMode observerMode, char name[16])
{
	switch (observerMode) {
	case ObserverMode::OBS_MODE_NONE:
		strcpy(name, xorstr_("None"));
		break;
	case ObserverMode::OBS_MODE_DEATHCAM:
		strcpy(name, xorstr_("Deathcam"));
		break;
	case ObserverMode::OBS_MODE_FREEZECAM:
		strcpy(name, xorstr_("Freezecam"));
		break;
	case ObserverMode::OBS_MODE_FIXED:
		strcpy(name, xorstr_("Fixed"));
		break;
	case ObserverMode::OBS_MODE_IN_EYE:
		strcpy(name, xorstr_("In Eye"));
		break;
	case ObserverMode::OBS_MODE_CHASE:
		strcpy(name, xorstr_("Chase"));
		break;
	case ObserverMode::OBS_MODE_ROAMING:
		strcpy(name, xorstr_("Roaming"));
		break;
	}
}

void Features::Visuals::SpectatorList::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	auto* localPlayer = GameCache::GetLocalPlayer();

	CBaseEntity* currentTarget = nullptr;
	if (localPlayer) {
		if (localPlayer->IsAlive())
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

		if (player->GetDormant() || !localPlayer->IsAlive())
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
		char name[16];
		LocalizeObserverMode(observerMode, name);
		sprintf(text, xorstr_("%s -> %s (%s)"), first.name, second.name, name);

		const ImVec2 size = ImGui::CalcTextSize(text);
		const ImVec2 position(displaySize.x - size.x - 10.0f, offset + 10.0f);

		ShadowString::AddText(drawList, position, currentTarget && currentTarget == target ? ImGuiColors::red : ImGuiColors::white, text);

		offset += ImGui::GetTextLineHeightWithSpacing();
	}
}

void Features::Visuals::SpectatorList::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::SpectatorList::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
END_SERIALIZED_STRUCT
