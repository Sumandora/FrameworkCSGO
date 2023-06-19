#ifndef FEATURES_LEGIT_TRIGGERBOT
#define FEATURES_LEGIT_TRIGGERBOT

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class Triggerbot : public Feature {
public:
	Triggerbot()
		: Feature(xorstr_("Legit"), xorstr_("Triggerbot"))
	{
	}

	bool enabled = false;
	int input = ImGuiKey_None;
	bool secondaryFireWithR8Revolver = true;
	bool friendlyFire = false;
	int maximalFlashAmount = 255;
	bool dontShootThroughSmoke = false;
	// TODO Delay

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} triggerbot;

#endif