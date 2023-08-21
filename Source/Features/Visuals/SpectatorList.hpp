#ifndef FEATURES_VISUALS_SPECTATORLIST
#define FEATURES_VISUALS_SPECTATORLIST

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CViewSetup.hpp"

#include "../../GUI/ImGuiColors.hpp"

inline class SpectatorList : public Feature {
public:
	SpectatorList()
		: Feature("Visuals", "Spectator list")
	{
	}

	bool enabled = false;
	bool showMode = true;
	bool showAllSpectators = true;
	ImColor otherTargetColor = ImGuiColors::white;
	ImColor sameTargetColor = ImGuiColors::red;

	void update() const;
	void imGuiRender(ImDrawList* drawList) const;

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} spectatorList;

#endif