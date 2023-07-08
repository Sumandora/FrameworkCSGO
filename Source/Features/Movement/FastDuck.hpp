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

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} fastDuck;

#endif