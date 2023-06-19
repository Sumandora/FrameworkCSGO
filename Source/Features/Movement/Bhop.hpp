#ifndef FEATURES_MOVEMENT_BHOP
#define FEATURES_MOVEMENT_BHOP

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class Bhop : public Feature {
public:
	Bhop()
		: Feature(xorstr_("Movement"), xorstr_("Bhop"))
	{
	}

	bool enabled = false;
	float hitChance = 1.0f;
	float inAirDistortion = 0.0f;
	int minimumReleaseDelay = 0;
	int maximumRelaseDelay = 0;
	bool onlyWhenFalling = false;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} bhop;

#endif