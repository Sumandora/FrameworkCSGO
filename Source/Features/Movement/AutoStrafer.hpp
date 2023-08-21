#ifndef FEATURES_MOVEMENT_AUTOSTRAFER
#define FEATURES_MOVEMENT_AUTOSTRAFER

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class AutoStrafer : public Feature {
public:
	AutoStrafer()
		: Feature("Movement", "Auto strafer")
	{
	}

	bool enabled = false;
	bool directional = true;
	bool allowHardTurns = false;
	float hardTurnThreshold = 135.0f;
	bool onlyWhenIdle = false;

	void createMove(CUserCmd* cmd);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} autoStrafer;

#endif