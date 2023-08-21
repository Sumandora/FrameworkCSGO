#ifndef FEATURES_GENERAL_WATERMARK
#define FEATURES_GENERAL_WATERMARK

#include "../Features.hpp"

#include "imgui.h"

inline class Watermark : public Feature {
public:
	Watermark()
		: Feature("General", "Watermark")
	{
	}

	bool enabled = true;

	void imGuiRender(ImDrawList* drawList);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} watermark;

#endif