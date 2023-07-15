#include "IgnoreSvPure.hpp"

#include "../../GUI/ImGuiColors.hpp"

void IgnoreSvPure::SetupGUI()
{
	ImGui::Checkbox("Cancel net message", &cancelNetMessage);
	ImGui::Checkbox("Allow third party files", &allowThirdPartyFiles);
}

SCOPED_SERIALIZER(IgnoreSvPure)
{
	SERIALIZE("Cancel net message", cancelNetMessage);
	SERIALIZE("Allow third party files", allowThirdPartyFiles);
}