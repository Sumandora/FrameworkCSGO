#include "FlashReduction.hpp"

#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../GUI/ImGuiColors.hpp"

// TODO Render a progress bar into the overlay which shows the serverside flash amount

bool FlashReduction::ViewDrawFade(std::byte* color, IMaterial*& pFadeMaterial)
{
	if (!enabled)
		return false;

	const char* currName = pFadeMaterial->GetName();
	if (strcmp(currName, "effects/flashbang") == 0) {
		return clearFlashbang;
	} else if (strcmp(currName, "effects/flashbang_white") == 0) {
		static IMaterial* white = Interfaces::materialSystem->FindMaterial("debug/debugdrawflat", nullptr);

		const int alpha = (int)color[0]; // RGB = alpha, alpha = 255 (don't ask me why they did it like that)

		color[0] = (std::byte)(255 * flashColor.Value.x);
		color[1] = (std::byte)(255 * flashColor.Value.y);
		color[2] = (std::byte)(255 * flashColor.Value.z);
		color[3] = (std::byte)((float)alpha * flashColor.Value.w);

		pFadeMaterial = white; // This material has no attributes throwing off the colors
	}
	return false;
}

void FlashReduction::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Checkbox("Clear flashbang", &clearFlashbang);
	ImGui::HelpMarker("Won't draw the screenshot, which was taken at the begin of the flash effect");
	ImGui::ClickableColorButton("Flash color", flashColor, ImGuiColorEditFlags_AlphaBar);
}

SCOPED_SERIALIZER(FlashReduction)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Clear flashbang", clearFlashbang);
	SERIALIZE_VECTOR4D("Flash color", flashColor.Value);
}
