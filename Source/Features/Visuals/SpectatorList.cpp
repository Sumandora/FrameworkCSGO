#include "Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"

#include "../../GameCache.hpp"
#include "../../GUI/ImGuiColors.hpp"
#include "../../Interfaces.hpp"

#include <vector>

static bool enabled = false;

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
			CBaseHandle* observerTarget = localPlayer->ObserverTarget();
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

		if (player->GetDormant() || player->IsAlive())
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(player->ObserverTarget());
		if (!target)
			continue;

		PlayerInfo first{};
		Interfaces::engine->GetPlayerInfo(i, &first);

		PlayerInfo second{};
		Interfaces::engine->GetPlayerInfo(target->entindex(), &second);

		const ObserverMode observerMode = *player->ObserverMode();
		const char* observerModeName = LocalizeObserverMode(observerMode);
		if (!observerModeName)
			return; // We don't have a name?

		char text[strlen(first.name) + 4 + strlen(second.name) + 2 + strlen(observerModeName) + 1];
		sprintf(text, xorstr_("%s -> %s (%s)"), first.name, second.name, observerModeName);

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
