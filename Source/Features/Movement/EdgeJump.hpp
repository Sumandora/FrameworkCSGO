#ifndef FEATURES_MOVEMENT_EDGEJUMP
#define FEATURES_MOVEMENT_EDGEJUMP

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CUserCmd.hpp"

#include "../../GUI/Elements/Keybind.hpp"

inline class EdgeJump : public Feature {
public:
	EdgeJump()
		: Feature("Movement", "Edge jump")
	{
	}

	bool enabled = false;
	Input input{};

	void createMove(CUserCmd* cmd);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} edgeJump;

#endif