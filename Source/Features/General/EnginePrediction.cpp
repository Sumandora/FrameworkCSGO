#include "EnginePrediction.hpp"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../GUI/ImGuiColors.hpp"
#include "../../Interfaces.hpp"
#include "../../Utils/Prediction.hpp"

#include <cstdio>
#include <cstring>

void EnginePrediction::ImGuiWarning()
{
	if (!enabled)
		ImGui::TextColored(ImGuiColors::yellow, "Warning: This feature expects engine prediction to be enabled");
}

static float previousVelocityModifer;

bool EnginePrediction::EmitSound(int iEntIndex, const char* pSoundEntry)
{
	if (dontEmitSoundsDuringPrediction) {
		if (Interfaces::prediction->m_bInPrediction && Interfaces::prediction->m_bIsFirstTimePredicted && iEntIndex == Interfaces::engine->GetLocalPlayer())
			return strcasestr(pSoundEntry, "land"); // TODO Better check
	}
	return false;
}

void EnginePrediction::StartPrediction(CUserCmd* cmd)
{
	moveData = {};
	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();

	if (!localPlayer || !localPlayer->IsAlive())
		return;

	prePredictionFlags = *localPlayer->Flags();
	prePredictionMoveType = static_cast<MoveType>(localPlayer->GetMoveType());

	previousVelocityModifer = *localPlayer->VelocityModifier();
	Utils::StartPrediction(cmd, moveData);
}

void EnginePrediction::EndPrediction()
{
	moveData = {};

	prePredictionFlags = 0;
	prePredictionMoveType = MOVETYPE_NONE;

	if (!enabled || !Interfaces::engine->IsInGame())
		return;

	Utils::EndPrediction();

	if (forceResetVelocityModifier) // I'm curious why people on UC seem to have so many problems figuring this out... Does this have some major downside, which I haven't noticed yet?
		*Memory::GetLocalPlayer()->VelocityModifier() = previousVelocityModifer;
}

void EnginePrediction::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	if (!enabled) {
		ImGui::Text("Warning: Disabling engine prediction may result in massive accuracy loss");
	}
	ImGui::Checkbox("Force reset velocity modifier", &forceResetVelocityModifier);
	ImGui::HelpMarker("This netvar is not being reset properly when predicting.\nNote that the \"fix\" is not intended by the engine.");
	ImGui::Checkbox("Don't emit sounds during prediction", &dontEmitSoundsDuringPrediction);
}

SCOPED_SERIALIZER(EnginePrediction)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Force reset velocity modifier", forceResetVelocityModifier);
	SERIALIZE("Don't emit sounds during prediction", dontEmitSoundsDuringPrediction);
}
