#ifndef FEATURES_MOVEMENT_JUMPBUG
#define FEATURES_MOVEMENT_JUMPBUG

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class JumpBug : public Feature {
public:
	JumpBug()
		: Feature(xorstr_("Movement"), xorstr_("Jump bug"))
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