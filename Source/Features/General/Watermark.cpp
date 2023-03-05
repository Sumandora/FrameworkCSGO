#include "Watermark.hpp"

#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"

bool Features::General::Watermark::enabled = true;

void Features::General::Watermark::ImGuiRender(ImDrawList* drawList) {
    if(!enabled)
        return;
    
    ShadowString::AddText(drawList, ImVec2(10.0f, 10.0f), ImColor(255, 255, 255, 255), "Framework");
}

void Features::General::Watermark::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##GeneralWatermark"), &enabled);
}
