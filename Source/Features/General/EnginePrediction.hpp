#ifndef FEATURES_GENERAL_ENGINEPREDICTION
#define FEATURES_GENERAL_ENGINEPREDICTION

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CMoveData.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/MoveType.hpp"

inline class EnginePrediction : public Feature {
public:
	EnginePrediction()
		: Feature("General", "Engine prediction")
	{
	}

	bool enabled = true;
	bool forceResetVelocityModifier = false;

	CMoveData moveData{};
	int prePredictionFlags = 0;
	MoveType prePredictionMoveType = MOVETYPE_NONE;
	void ImGuiWarning();

	void StartPrediction(CUserCmd* cmd);
	void EndPrediction();

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} enginePrediction;

#endif