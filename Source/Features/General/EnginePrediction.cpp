#include "../General.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GameCache.hpp"
#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../GUI/ImGuiColors.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"
#include "../../Utils/Prediction.hpp"

#include <cstring>

bool Features::General::EnginePrediction::enabled = true;
static bool forceResetVelocityModifier = false;

CMoveData Features::General::EnginePrediction::moveData {};
int Features::General::EnginePrediction::prePredictionFlags = 0;
MoveType Features::General::EnginePrediction::prePredictionMoveType = MOVETYPE_NONE;

void Features::General::EnginePrediction::ImGuiWarning()
{
	if (!enabled)
		ImGui::TextColored(ImGuiColors::yellow, xorstr_("Warning: This feature expects engine prediction to be enabled"));
}

static float previousVelocityModifer;

void Features::General::EnginePrediction::StartPrediction(CUserCmd* cmd)
{
	moveData = {};
	if (!enabled)
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();

	if (!localPlayer || !localPlayer->IsAlive())
		return;

	prePredictionFlags = *localPlayer->Flags();
	prePredictionMoveType = static_cast<MoveType>(localPlayer->GetMoveType());

	previousVelocityModifer = *localPlayer->VelocityModifier();
	Utils::StartPrediction(cmd, moveData);
}

void Features::General::EnginePrediction::EndPrediction()
{
	moveData = {};

	prePredictionFlags = 0;
	prePredictionMoveType = MOVETYPE_NONE;

	if (!enabled)
		return;

	Utils::EndPrediction();

	if (forceResetVelocityModifier) // I'm curious why people on UC seem to have so many problems figuring this out... Does this have some major downside, which I haven't noticed yet?
		*GameCache::GetLocalPlayer()->VelocityModifier() = previousVelocityModifer;
}

void Features::General::EnginePrediction::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	if (!enabled) {
		ImGui::Text(xorstr_("Warning: Disabling engine prediction may result in massive accuracy loss"));
	}
	ImGui::Checkbox(xorstr_("Force reset velocity modifier"), &forceResetVelocityModifier);
	ImGui::HelpMarker(xorstr_("This netvar is not being reset properly when predicting.\nNote that the \"fix\" is not intended by the engine."));
}

BEGIN_SERIALIZED_STRUCT(Features::General::EnginePrediction::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
END_SERIALIZED_STRUCT
