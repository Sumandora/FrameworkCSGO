#ifndef FEATURES_MOVEMENT_AUTOSTRAFER
#define FEATURES_MOVEMENT_AUTOSTRAFER

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class AutoStrafer : public Feature {
public:
	AutoStrafer()
		: Feature(xorstr_("Movement"), xorstr_("Auto strafer"))
	{
	}

	bool enabled = false;
	bool directional = true;
	bool allowHardTurns = false;
	float hardTurnThreshold = 135.0f;
	bool onlyWhenIdle = false;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} autoStrafer;

#endif