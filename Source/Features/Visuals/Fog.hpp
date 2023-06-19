#ifndef FEATURES_VISUALS_FOG
#define FEATURES_VISUALS_FOG

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/Math/Vector.hpp"

#include "../../GUI/ImGuiColors.hpp"

inline class Fog : public Feature {
public:
	Fog()
		: Feature(xorstr_("Visuals"), xorstr_("Fog"))
	{
	}

	bool enabled = false;
	bool hideFog = false;
	ImColor colorPrimary = ImGuiColors::white;
	bool blend = false;
	Vector dirPrimary = {};
	ImColor colorSecondary = ImGuiColors::black;
	float start = 0.0f;
	float end = 0.0f;
	float farZ = 0.0f;
	float maxDensity = 1.0f;
	float hdrColorScale = 1.0f;
	float zoomFogScale = 1.0f;

	void FrameStageNotify();

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} fog;

#endif