#include "EnginePrediction.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Utils/Prediction.hpp"
#include "../../GameCache.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include <cstring>

bool Features::General::EnginePrediction::enabled = true;

CMoveData Features::General::EnginePrediction::moveData {};
int Features::General::EnginePrediction::prePredictionFlags = 0;

void Features::General::EnginePrediction::StartPrediction(CUserCmd* cmd)
{
	moveData = {};
	if (!enabled)
		return;
	prePredictionFlags = *GameCache::GetLocalPlayer()->Flags();
	Utils::StartPrediction(cmd, moveData);
}

void Features::General::EnginePrediction::EndPrediction()
{
	Utils::EndPrediction();
	prePredictionFlags = 0;
}

void Features::General::EnginePrediction::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	if (!enabled) {
		ImGui::Text(xorstr_("Warning: Disabling engine prediction may result in massive accuracy loss"));
	}
}

BEGIN_SERIALIZED_STRUCT(Features::General::EnginePrediction::Serializer, xorstr_("Engine prediction"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
END_SERIALIZED_STRUCT
