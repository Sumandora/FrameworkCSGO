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
	if (!enabled)
		return;

	if (std::strcmp(soundEntry, "UIPanorama.popup_accept_match_beep") == 0) {
		// Found game!
		eventLog.CreateReport("Found a game!");

		if (sendSystemNotification) {
			// If we are not auto accepting, we should tell the user that it's urgent, don't set the urgency level however, since a game shouldn't make urgent notifications
			if (dontAccept)
				(void)std::system("notify-send \"Matchmaking Game Found. Please accept promptly to join the match.\"");
			else
				(void)std::system("notify-send \"Matchmaking Game Found.\"");
		}

		if (!dontAccept) {
			Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", "$.DispatchEvent(\"MatchAssistedAccept\");");
		}
	}
}

void AutoAccept::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Checkbox("Send system notification", &sendSystemNotification);
	ImGui::Checkbox("Don't accept", &dontAccept);
	ImGui::HelpMarker("This may be useful in case you only want the notification");
}

SCOPED_SERIALIZER(AutoAccept)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Send system notification", sendSystemNotification);
	SERIALIZE("Don't accept", dontAccept);
}
