#include "Watermark.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

void Watermark::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled)
		return;

	ShadowString::AddText(drawList, ImVec2(10.0f, 10.0f), ImGuiColors::white,
		"Framework"
#ifdef DEBUG
		" (debug build)"
#endif
	);
}

void Watermark::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
}

SCOPED_SERIALIZER(Watermark)
{
	SERIALIZE("Enabled", enabled);
}