#ifndef FEATURES_MOVEMENT_JUMPBUG
#define FEATURES_MOVEMENT_JUMPBUG

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class JumpBug : public Feature {
public:
	JumpBug()
		: Feature("Movement", "Jump bug")
	{
	}

	bool enabled = false;
	int input = ImGuiKey_None;
	bool preDuck = false;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} jumpBug;

#endif