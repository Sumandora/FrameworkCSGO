#ifndef FEATURES_VISUALS_FOV
#define FEATURES_VISUALS_FOV

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CViewSetup.hpp"

#include "../../GUI/ImGuiColors.hpp"

inline class FOV : public Feature {
public:
	FOV()
		: Feature("Visuals", "FOV") // TODO Rename to "Field of view"?
	{
	}

	bool forceFOV = false;
	bool ignoreScoped = true;
	float fov = 90.0f;
	bool forceViewModel = false;
	float viewModelFovOffset = 0.0f;
	bool viewOffset = true;
	Vector offset;
	Vector rotationOffset;

	void OverrideView(CViewSetup* pSetup);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} fov;

#endif