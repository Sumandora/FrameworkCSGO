#include "FastDuck.hpp"

#include "imgui.h"

#include "../../SDK/Definitions/InputFlags.hpp"

void FastDuck::createMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	cmd->buttons |= IN_BULLRUSH;

	// TODO Fake Duck
}

void FastDuck::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::Text("Warning: This feature is known to cause untrusted bans");
}

SCOPED_SERIALIZER(FastDuck)
{
	SERIALIZE("Enabled", enabled);
}
