#ifndef FEATURES_MOVEMENT_EDGEJUMP
#define FEATURES_MOVEMENT_EDGEJUMP

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class EdgeJump : public Feature {
public:
	EdgeJump()
		: Feature(xorstr_("Movement"), xorstr_("Edge jump"))
	{
	}

	bool enabled = false;
	int input = ImGuiKey_None;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} edgeJump;

#endif