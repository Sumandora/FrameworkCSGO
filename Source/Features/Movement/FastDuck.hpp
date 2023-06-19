#ifndef FEATURES_MOVEMENT_FASTDUCK
#define FEATURES_MOVEMENT_FASTDUCK

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/CUserCmd.hpp"

inline class FastDuck : public Feature {
public:
	FastDuck()
		: Feature(xorstr_("Movement"), xorstr_("Fast Duck"))
	{
	}

	bool enabled = false;

	void CreateMove(CUserCmd* cmd);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} fastDuck;

#endif