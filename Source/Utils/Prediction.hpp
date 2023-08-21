#ifndef UTILS_PREDICTION
#define UTILS_PREDICTION

#include "../SDK/GameClass/CMoveData.hpp"
#include "../SDK/GameClass/CUserCmd.hpp"

namespace Utils {
	bool startPrediction(CUserCmd* cmd, CMoveData& moveData);
	void endPrediction();
}

#endif