#include "../Movement.hpp"

#include "imgui.h"

#include "xorstr.hpp"

#include "../../SDK/Definitions/InputFlags.hpp"

static bool enabled = false;

void Features::Movement::FastDuck::CreateMove(CUserCmd* cmd)
{
	if (!enabled)
		return;

	cmd->buttons |= IN_BULLRUSH;

	// TODO Fake Duck
}

void Features::Movement::FastDuck::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	ImGui::Text(xorstr_("Warning: This feature is known to cause untrusted bans"));
}

BEGIN_SERIALIZED_STRUCT(Features::Movement::FastDuck::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
END_SERIALIZED_STRUCT
