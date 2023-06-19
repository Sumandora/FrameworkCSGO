#include "FastDuck.hpp"

#include "imgui.h"

#include "xorstr.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

void FastDuck::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	cmd->buttons |= IN_BULLRUSH;

	// TODO Fake Duck
}

void FastDuck::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::Text(xorstr_("Warning: This feature is known to cause untrusted bans"));
}

SCOPED_SERIALIZER(FastDuck)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
}
