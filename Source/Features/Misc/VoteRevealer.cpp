#include "VoteRevealer.hpp"

#include "../../Interfaces.hpp"
#include "../General/EventLog.hpp"

#include "imgui.h"

#include <cstring>
#include <string>

static std::string FormatVote(int vote)
{
	switch (vote) {
	case 0:
		return "Yes";
	case 1:
		return "No";
	default: // This case shouldn't happen in vanilla csgo
		return "Answer #" + std::to_string(vote);
	}
}

void VoteRevealer::FireEvent(CGameEvent* gameEvent)
{
	if (strcmp(gameEvent->GetName(), "vote_cast") != 0)
		return;

	PlayerInfo info;
	if (!Interfaces::engine->GetPlayerInfo(gameEvent->GetInt("entityid"), &info))
		return; // Weird?

	eventLog.CreateReport("%s voted %s", info.name, FormatVote(gameEvent->GetInt("vote_option")).c_str());
}

void VoteRevealer::SetupGUI()
{
	ImGui::Checkbox("Reveal voters", &revealVoters);
}

SCOPED_SERIALIZER(VoteRevealer)
{
	SERIALIZE("Reveal voters", revealVoters);
}