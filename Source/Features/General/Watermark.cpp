#include "Watermark.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

void Watermark::ImGuiRender(ImDrawList* drawList)
{
	if (!enabled)
		return;

	ShadowString::AddText(drawList, ImVec2(10.0f, 10.0f), ImGuiColors::white,
		xorstr_(
#ifdef DEBUG
			"Framework (debug build)"
#else
			"Framework"
#endif
			));
}

void Watermark::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
}

SCOPED_SERIALIZER(Watermark)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
}