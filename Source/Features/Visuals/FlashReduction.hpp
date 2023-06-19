#ifndef FEATURES_VISUALS_FLASHREDUCTION
#define FEATURES_VISUALS_FLASHREDUCTION

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../SDK/GameClass/IMaterial.hpp"

#include "../../GUI/ImGuiColors.hpp"

inline class FlashReduction : public Feature {
public:
	FlashReduction()
		: Feature(xorstr_("Visuals"), xorstr_("Flash reduction"))
	{
	}

	bool enabled = false;
	bool clearFlashbang = false;
	ImColor flashColor = ImGuiColors::white;

	bool ViewDrawFade(std::byte* color, IMaterial*& pFadeMaterial);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} flashReduction;

#endif