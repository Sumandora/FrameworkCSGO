#ifndef FEATURES_MOVEMENT_FASTDUCK
#define FEATURES_MOVEMENT_FASTDUCK

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class FastDuck : public Feature {
public:
	FastDuck()
		: Feature("Movement", "Fast Duck")
	{
	}

	bool enabled = false;

	void createMove(CUserCmd* cmd);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} fastDuck;

#endif