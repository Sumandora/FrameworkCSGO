#ifndef UTILS_PREDICTION
#define UTILS_PREDICTION

#include "../SDK/CUserCmd.hpp"
#include "../SDK/GameClass/CMoveData.hpp"

namespace Utils {
    bool StartPrediction(CUserCmd* cmd, CMoveData& moveData);
    void EndPrediction();
}

#endif