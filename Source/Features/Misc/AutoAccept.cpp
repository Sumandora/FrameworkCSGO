#include "AutoAccept.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "../General/EventLog.hpp"

#include "../../Memory.hpp"

#include "../../Interfaces.hpp"

#include <cstdlib>
#include <cstring>

// TODO Only once per game? right now we do it every beep sound

void AutoAccept::EmitSound(const char* soundEntry)
{
	if (!sendSystemNotification && !autoAccept)
		return;

	if (std::strcmp(soundEntry, "UIPanorama.popup_accept_match_beep") == 0) {
		// Found game!
		eventLog.CreateReport("Found a game!");

		if (sendSystemNotification) {
			// If we are not auto accepting, we should tell the user that it's urgent, don't set the urgency level however, since a game shouldn't make urgent notifications
			(void)std::system(autoAccept ? "notify-send \"Matchmaking Game Accepted.\""
										 : "notify-send \"Matchmaking Game Found. Please accept promptly to join the match.\"");
		}

		if (autoAccept) {
			Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", "$.DispatchEvent(\"MatchAssistedAccept\");");
		}
	}
}

void AutoAccept::SetupGUI()
{
	ImGui::Checkbox("Send system notification", &sendSystemNotification);
	ImGui::Checkbox("Auto accept", &autoAccept);
	ImGui::HelpMarker("Turn off in case you only want the notification");
}

SCOPED_SERIALIZER(AutoAccept)
{
	SERIALIZE("Send system notification", sendSystemNotification);
	SERIALIZE("Auto accept", autoAccept);
}
