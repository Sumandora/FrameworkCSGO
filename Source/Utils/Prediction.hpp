#ifndef UTILS_PREDICTION
#define UTILS_PREDICTION

#include "SDK/GameClass/CMoveData.hpp"
#include "SDK/GameClass/CUserCmd.hpp"

namespace Utils {
	bool StartPrediction(CUserCmd* cmd, CMoveData& moveData);
	void EndPrediction();
}

#endif