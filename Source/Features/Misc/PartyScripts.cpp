#include "PartyScripts.hpp"

#include "imgui.h"

#include "../../GUI/ImGuiColors.hpp"
#include "../../Interfaces.hpp"

#include <cstdlib>
#include <sstream>
#include <string>

// https://github.com/seksea/eclipse-luas/blob/master/lobbytricks.lua
static void SpawnPopup(size_t amount)
{
	std::stringstream ss{};
	ss << "for(var i = 0; i < " << amount << "; i++) ";
	ss << "PartyListAPI.SessionCommand(\"Game::HostEndGamePlayAgain\", \"run all xuid ${MyPersonaAPI.GetXuid()}\");";
	std::string str = ss.str();
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", str.c_str());
}

static void ClosePopups()
{
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", "$.DispatchEvent(\"CSGOHideMainMenu\");");
}

// strings client_client.so | grep "SFUI_QMM_ERROR_"
// I don't know any better way of getting them
static const char* errorMessages[]{
	"X_NotLoggedIn",
	"1_NotLoggedIn",
	"X_FailReadyUp",
	"1_FailReadyUp",
	"X_VacBanned",
	"1_VacBanned",
	"NotLoggedIn",
	"PartyTooLarge",
	"VacBanned",
	"X_InsecureBlocked",
	"X_PerfectWorldRequired",
	"X_PerfectWorldDenied",
	"X_InsufficientLevelNP",
	"1_InsufficientLevelNP",
	"NotVacVerified",
	"X_OngoingMatch",
	"1_OngoingMatch",
	"X_NotVacVerified",
	"1_NotVacVerified",
	"X_FailPingServer",
	"1_FailPingServer",
	"1_FailReadyUp_Title",
	"X_FailVerifyClan",
	"1_FailVerifyClan",
	"1_FailVerifyClan_Title",
	"1_VacBanned_Title",
	"X_PenaltySeconds",
	"1_PenaltySeconds",
	"X_PenaltySecondsGreen",
	"1_PenaltySecondsGreen",
	"X_InsufficientLevel02",
	"X_InsufficientLevel03",
	"X_InsufficientLevel04",
	"X_InsufficientLevel05",
	"X_InsufficientLevel",
	"1_InsufficientLevel02",
	"1_InsufficientLevel03",
	"1_InsufficientLevel04",
	"1_InsufficientLevel05",
	"1_InsufficientLevel",
	"NoOngoingMatch",
	"PartyRequired1",
	"PartyRequired2",
	"PartyRequired3",
	"PartyRequired4",
	"PartyRequired5",
	"ClientUpdateRequired",
	"ClientBetaVersionMismatch",
	"FailedToSetupSearchData",
	"FailedToPingServers",
	"SkillGroupMissing",
	"SkillGroupLargeDelta",
	"FailedToReadyUp",
	"FailedToVerifyClan",
	"InvalidGameMode",
	"UnavailMapSelect",
	"OperationPassFullTeam",
	"OperationPassInvalid",
	"OperationQuestIdInactive",
	"FullTeamRequired",
	"TournamentTeamAccounts",
	"TournamentTeamSize",
	"TournamentMatchRequired",
	"TournamentMatchNoEvent",
	"TournamentMatchInvalidEvent",
	"TournamentMatchSetupYourTeam",
	"TournamentMatchSetupSameTeam",
	"TournamentMatchSetupNoTeam",
	"X_AccountWarningTrustMinor",
	"X_AccountWarningNonPrime",
	"X_AccountWarningTrustMajor",
	"X_AccountWarningTrustMajor_Summary",
	"X_PureFileStateDirty",
	"X_SearchTimeExceeded"
};

static void CreateErrorMessage(size_t amount, const char* type)
{
	std::stringstream ss{};
	ss << "for(var i = 0; i < " << amount << "; i++) ";
	ss << "PartyListAPI.SessionCommand(\"Game::ChatReportError\", \"run all xuid ${MyPersonaAPI.GetXuid()} error #SFUI_QMM_ERROR_" << type << "\");";
	std::string str = ss.str();
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", str.c_str());
}

static void CancelMatchmaking()
{
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", "LobbyAPI.StopMatchmaking();");
}

static void CancelMatchmakingIfSearching()
{
	Interfaces::panoramaUIEngine->AccessUIEngine()->RunScript("CSGOMainMenu", "if (LobbyAPI.GetMatchmakingStatusString() == \"#SFUI_QMM_State_find_searching\") { LobbyAPI.StopMatchmaking(); }");
}

void PartyScripts::PerformPartyScripts()
{
	if (!autoClosePopups && !autoErrorCreation && !autoCancelMatchmaking)
		return;

	if (Interfaces::engine->IsInGame())
		return; // Executing scripts while in game leads to "Maximum call stack reached" errors...

	if (autoClosePopups) {
		static float lastUpdate = 0.0f;

		if (Memory::globalVars->curtime - lastUpdate > popupInterval) {
			ClosePopups();
			lastUpdate = Memory::globalVars->curtime;
		}
	}
	if (autoErrorCreation) {
		static float lastUpdate = 0.0f;

		if (Memory::globalVars->curtime - lastUpdate > errorInterval) {
			if (!randomErrors) {
				CreateErrorMessage(errorAmount, errorMessages[selectedError]);
			} else {
				for (int i = 0; i < errorAmount; i++)
					CreateErrorMessage(1, errorMessages[rand() % IM_ARRAYSIZE(errorMessages)]);
			}
			lastUpdate = Memory::globalVars->curtime;
		}
	}
	if (autoCancelMatchmaking) {
		static float lastUpdate = 0.0f;

		if (Memory::globalVars->curtime - lastUpdate > cancelInterval) {
			if (!onlyWhenSearching)
				CancelMatchmaking();
			else
				CancelMatchmakingIfSearching();
			lastUpdate = Memory::globalVars->curtime;
		}
	}
}

void PartyScripts::SetupGUI()
{
	if (Interfaces::engine->IsInGame())
		ImGui::TextColored(ImGuiColors::yellow, "Executing scripts in game may lead to bugs/crashes");

	ImGui::SliderInt("Popup amount", &popupAmount, 1, 200);
	if (ImGui::Button("Spawn popups")) {
		SpawnPopup(popupAmount);
	}

	if (ImGui::Button("Close all popups")) {
		ClosePopups();
	}

	ImGui::Checkbox("Auto close popups", &autoClosePopups);
	if (autoClosePopups)
		ImGui::SliderFloat("Popup interval", &popupInterval, 0.0f, 5.0f);

	ImGui::Separator();

	ImGui::Combo("Error type", &selectedError, errorMessages, IM_ARRAYSIZE(errorMessages));

	ImGui::SliderInt("Error amount", &errorAmount, 1, 200);
	if (ImGui::Button("Create error messages")) {
		CreateErrorMessage(errorAmount, errorMessages[selectedError]);
	}

	if (ImGui::Button("Create random error messages")) {
		for (int i = 0; i < errorAmount; i++) {
			CreateErrorMessage(1, errorMessages[rand() % IM_ARRAYSIZE(errorMessages)]);
		}
	}

	ImGui::Checkbox("Auto error creation", &autoErrorCreation);
	if (autoErrorCreation) {
		ImGui::SliderFloat("Error interval", &errorInterval, 0.0f, 5.0f);
		ImGui::Checkbox("Random errors", &randomErrors);
	}

	ImGui::Separator();

	if (ImGui::Button("Cancel matchmaking")) {
		CancelMatchmaking();
	}

	ImGui::Checkbox("Auto cancel matchmaking", &autoCancelMatchmaking);
	if (autoCancelMatchmaking) {
		ImGui::SliderFloat("Cancel interval", &cancelInterval, 0.0f, 5.0f);
		ImGui::Checkbox("Only when searching", &onlyWhenSearching);
	}
}

SCOPED_SERIALIZER(PartyScripts)
{
	SERIALIZE("Popup amount", popupAmount);
	SERIALIZE("Auto close popups", autoClosePopups);
	SERIALIZE("Popup interval", popupInterval);

	SERIALIZE("Error amount", errorAmount);
	SERIALIZE("Selected error", selectedError);

	SERIALIZE("Auto error creation", autoErrorCreation);
	SERIALIZE("Error interval", errorInterval);
	SERIALIZE("Random errors", randomErrors);

	SERIALIZE("Auto cancel matchmaking", autoCancelMatchmaking);
	SERIALIZE("Cancel interval", cancelInterval);
	SERIALIZE("Only when searching", onlyWhenSearching);
}
