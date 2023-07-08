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
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;
	float rotationOffsetX = 0.0f;
	float rotationOffsetY = 0.0f;
	float rotationOffsetZ = 0.0f;

	void OverrideView(CViewSetup* pSetup);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} fov;

#endif