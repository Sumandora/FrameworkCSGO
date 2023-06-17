#include "Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/ShadowString.hpp"

#include "../../GUI/ImGuiColors.hpp"
#include "../../Interfaces.hpp"

#include <vector>

static bool enabled = false;
static bool showMode = true;
static bool showAllSpectators = true;
static ImColor otherTargetColor = ImGuiColors::white;
static ImColor sameTargetColor = ImGuiColors::red;

struct SpectatorEntry {
	int spectator;
	int spectated;
	ObserverMode mode;
	bool targeted; // Is spectated = localPlayer or spectated = spectated by localPlayer
};

std::vector<SpectatorEntry> spectators;

void Features::Visuals::SpectatorList::Update()
{
	spectators.clear();

	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();

	CBaseEntity* currentTarget = nullptr;
	if (localPlayer) {
		if (localPlayer->IsAlive())
			currentTarget = localPlayer;
		else {
			CBaseHandle* observerTarget = localPlayer->ObserverTarget();
			if (*observerTarget != INVALID_EHANDLE_INDEX)
				currentTarget = Interfaces::entityList->GetClientEntityFromHandle(observerTarget);
		}
	}

	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player)
			continue;

		if (player->GetDormant() || player->IsAlive())
			continue;

		CBaseEntity* target = Interfaces::entityList->GetClientEntityFromHandle(player->ObserverTarget());
		if (!target)
			continue;

		if (!showAllSpectators && target != currentTarget)
			continue;

		spectators.push_back({ i, target->entindex(), *player->ObserverMode(), target == currentTarget });
	}
}

void Features::Visuals::SpectatorList::ImGuiRender(ImDrawList* drawList)
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

		const char* unknownObserverMode = xorstr_("Unknown observer mode"); // Initialize it before to prevent dangling pointers
		const char* observerModeName = LocalizeObserverMode(spectator.mode);
		if (!observerModeName)
			observerModeName = unknownObserverMode;

		std::stringstream ss;

		ss << first.name;
		if (showAllSpectators)
			ss << xorstr_(" -> ") << second.name;
		if (showMode)
			ss << xorstr_(" (") << observerModeName << xorstr_(")");

		const std::string str = ss.str();
		const char* text = str.c_str();

		const ImVec2 size = ImGui::CalcTextSize(text);
		const ImVec2 position(displaySize.x - size.x - 10.0f, offset + 10.0f);

		ShadowString::AddText(drawList, position, spectator.targeted ? sameTargetColor : otherTargetColor, text);

		offset += ImGui::GetTextLineHeightWithSpacing();
	}
}

void Features::Visuals::SpectatorList::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::Checkbox(xorstr_("Show mode"), &showMode);
	ImGui::Checkbox(xorstr_("Show all spectators"), &showAllSpectators);
	if (showAllSpectators)
		ImGui::ClickableColorButton(xorstr_("Other target color"), otherTargetColor);
	ImGui::ClickableColorButton(xorstr_("Same target color"), sameTargetColor);
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::SpectatorList::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Show mode"), showMode)
SERIALIZED_TYPE(xorstr_("Show all spectators"), showAllSpectators)
SERIALIZED_TYPE(xorstr_("Other target color"), otherTargetColor)
SERIALIZED_TYPE(xorstr_("Same target color"), sameTargetColor)
END_SERIALIZED_STRUCT
