#ifndef FEATURES_MOVEMENT_CROUCHJUMP
#define FEATURES_MOVEMENT_CROUCHJUMP

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class CrouchJump : public Feature {
public:
	CrouchJump()
		: Feature(xorstr_("Movement"), xorstr_("Crouch jump"))
	{
	}

	bool enabled = false;
	int input = ImGuiKey_None;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} crouchJump;

#endif