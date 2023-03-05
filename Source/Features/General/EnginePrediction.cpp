#include "EnginePrediction.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../Utils/Prediction.hpp"

#include <cstring>

bool Features::General::EnginePrediction::enabled = true;

CMoveData Features::General::EnginePrediction::moveData{};

void Features::General::EnginePrediction::StartPrediction(CUserCmd* cmd) {
    moveData = {};
    if(!enabled)
        return;
    Utils::StartPrediction(cmd, moveData);
}

void Features::General::EnginePrediction::EndPrediction() {
    Utils::EndPrediction();
}

void Features::General::EnginePrediction::SetupGUI() {
    ImGui::Checkbox(xorstr_("Enabled##GeneralEnginePrediction"), &enabled);
    if(!enabled) {
        ImGui::Text(xorstr_("Warning: Disabling engine prediction may result in massive accuracy loss"));
    }
}