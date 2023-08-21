#include "SpectatorList.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/ShadowString.hpp"

#include "../../GUI/ImGuiColors.hpp"
#include "../../Interfaces.hpp"

#include <sstream>
#include <vector>

struct SpectatorEntry {
	int spectator;
	int spectated;
	ObserverMode mode;
	bool targeted; // Is spectated = localPlayer or spectated = spectated by localPlayer
};

std::vector<SpectatorEntry> spectators;

void SpectatorList::update() const
{
	spectators.clear();

	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::getLocalPlayer();

	CBaseEntity* currentTarget = nullptr;
	if (localPlayer) {
		if (localPlayer->IsAlive())
			currentTarget = localPlayer;
		else {
			CBaseHandle* observerTarget = localPlayer->ObserverTarget();
			if (*observerTarget != invalidEHandleIndex)
				currentTarget = Interfaces::entityList->GetClientEntityFromHandle(observerTarget);
		}
	}

	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player)
			continue;

		if (player->IsAlive() && *player->Team() != TeamID::TEAM_SPECTATOR)
			continue;

		CBaseHandle* handle = player->ObserverTarget();
		if (*handle == invalidEHandleIndex)
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(handle);
		if (!target)
			continue;

		if (!showAllSpectators && target != currentTarget)
			continue;

		spectators.push_back({ i, target->entindex(), *player->ObserverMode(), target == currentTarget });
	}
}

void SpectatorList::imGuiRender(ImDrawList* drawList) const
{
	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	float offset = 0;

	for (const SpectatorEntry& spectator : spectators) {
		PlayerInfo first{};
		if (!Interfaces::engine->GetPlayerInfo(spectator.spectator, &first))
			continue;

		PlayerInfo second{};
		if (!Interfaces::engine->GetPlayerInfo(spectator.spectated, &second))
			continue;

		const char* observerModeName = LocalizeObserverMode(spectator.mode);
		if (!observerModeName)
			observerModeName = "Unknown observer mode";

		std::stringstream ss;

		ss << first.name;
		if (showAllSpectators)
			ss << " -> " << second.name;
		if (showMode)
			ss << " (" << observerModeName << ")";

		const std::string str = ss.str();
		const char* text = str.c_str();

		const ImVec2 size = ImGui::CalcTextSize(text);
		const ImVec2 position(displaySize.x - size.x - 10.0f, offset + 10.0f);

		ShadowString::AddText(drawList, position, spectator.targeted ? sameTargetColor : otherTargetColor, text);

		offset += ImGui::GetTextLineHeightWithSpacing();
	}
}

void SpectatorList::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Checkbox("Show mode", &showMode);
	ImGui::Checkbox("Show all spectators", &showAllSpectators);
	if (showAllSpectators)
		ImGui::ClickableColorButton("Other target color", otherTargetColor);
	ImGui::ClickableColorButton("Same target color", sameTargetColor);
}

SCOPED_SERIALIZER(SpectatorList)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Show mode", showMode);
	SERIALIZE("Show all spectators", showAllSpectators);
	SERIALIZE_VECTOR4D("Other target color", otherTargetColor.Value);
	SERIALIZE_VECTOR4D("Same target color", sameTargetColor.Value);
}
