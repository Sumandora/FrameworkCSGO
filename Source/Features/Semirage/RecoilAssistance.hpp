#ifndef FEATURES_SEMIRAGE_RECOILASSISTANCE
#define FEATURES_SEMIRAGE_RECOILASSISTANCE

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class RecoilAssistance : public Feature {
public:
	RecoilAssistance()
		: Feature("Semirage", "Recoil assistance")
	{
	}

	bool enabled = false;
	float horizontalAdjustment = 1.0f;
	float verticalAdjustment = 1.0f;
	bool silent = false;
	int minShots = 1;

	bool createMove(CUserCmd* cmd);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} recoilAssistance;

#endif