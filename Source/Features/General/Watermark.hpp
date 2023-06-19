#ifndef FEATURES_GENERAL_WATERMARK
#define FEATURES_GENERAL_WATERMARK

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

inline class Watermark : public Feature {
public:
	Watermark()
		: Feature(xorstr_("General"), xorstr_("Watermark"))
	{
	}

	bool enabled = true;

	void ImGuiRender(ImDrawList* drawList);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} watermark;

#endif