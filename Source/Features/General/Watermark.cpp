#include "Watermark.hpp"

#include "xorstr.hpp"

#include "../../GUI/ImGuiColors.hpp"
#include "../../GUI/Elements/ShadowString.hpp"

bool Features::General::Watermark::enabled = true;

void Features::General::Watermark::ImGuiRender(ImDrawList* drawList) {
    if(!enabled)
        return;
    
    ShadowString::AddText(drawList, ImVec2(10.0f, 10.0f), ImGuiColors::white, xorstr_(
#ifdef DEBUG
        "Framework (debug build)"
#else
        "Framework"
#endif
    ));
}

void Features::General::Watermark::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
}
