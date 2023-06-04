#include "Visuals.hpp"

#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../SDK/GameClass/CFogController.hpp"

static bool enabled = false;
static bool hideFog = false;
static ImColor colorPrimary = ImGuiColors::white;
static bool blend = false;
static Vector dirPrimary = {};
static ImColor colorSecondary = ImGuiColors::black;
static float start = 0.0f;
static float end = 0.0f;
static float farZ = 0.0f;
static float maxDensity = 1.0f;
static float hdrColorScale = 1.0f;
static float zoomFogScale = 1.0f;

// TODO Reset the fog when turned off
// TODO also some maps might not have a fog controller (?)

void Features::Visuals::Fog::FrameStageNotify()
{
	if (!enabled)
		return;

	for (int index = 0; index <= Interfaces::entityList->GetHighestEntityIndex(); index++) {
		CBaseEntity* entity = Interfaces::entityList->GetClientEntity(index);
		if (!entity)
			continue;

		ClientClass* clientClass = entity->GetClientClass();
		if (clientClass->m_ClassID != ClientClassID::CFogController)
			continue;

		auto* fogController = static_cast<CFogController*>(entity);

		*fogController->Enable() = !hideFog;
		if (!hideFog) {
			*fogController->Blend() = blend;
			*fogController->ColorPrimary() = ImGui::ColorConvertFloat4ToU32(colorPrimary.Value);
			if (blend) {
				*fogController->DirPrimary() = dirPrimary;
				*fogController->ColorSecondary() = ImGui::ColorConvertFloat4ToU32(colorSecondary.Value);
			}
			*fogController->Start() = start;
			*fogController->End() = end;
			*fogController->FarZ() = farZ;
			*fogController->MaxDensity() = maxDensity;

			*fogController->Duration() = 0.0f; // Disable transitions

			*fogController->HDRColorScale() = hdrColorScale;
			*fogController->ZoomFogScale() = zoomFogScale;
		}
	}
}

void Features::Visuals::Fog::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);

	ImGui::Separator();

	ImGui::Checkbox(xorstr_("Hide fog"), &hideFog);

	if (hideFog)
		return;

	ImGui::ClickableColorButton(xorstr_("Color primary"), colorPrimary);

	ImGui::Checkbox(xorstr_("Blend"), &blend);

	if (blend) {
		ImGui::DragFloat3(xorstr_("Dir Primary"), dirPrimary.Base(), 0.01f, -1.0f, 1.0f);
		ImGui::ClickableColorButton(xorstr_("Color secondary"), colorSecondary);
	}

	ImGui::SliderFloat(xorstr_("Start"), &start, 0.0f, 1024.0f, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("End"), &end, 0.0f, 1024.0f, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("Far z"), &farZ, 0.0f, 1024.0f * 16, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("Max density"), &maxDensity, 0.0f, 1.0f, xorstr_("%.2f"));

	ImGui::SliderFloat(xorstr_("HDR color scale"), &hdrColorScale, 0.0f, 1.0f, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("Zoom fog scale"), &zoomFogScale, 0.0f, 1.0f, xorstr_("%.2f"));
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::Fog::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Hide fog"), hideFog)
SERIALIZED_TYPE(xorstr_("Color primary"), colorPrimary)
SERIALIZED_TYPE(xorstr_("Blend"), blend)
SERIALIZED_TYPE(xorstr_("Dir Primary"), dirPrimary)
SERIALIZED_TYPE(xorstr_("Color secondary"), colorSecondary)
SERIALIZED_TYPE(xorstr_("Start"), start)
SERIALIZED_TYPE(xorstr_("End"), end)
SERIALIZED_TYPE(xorstr_("Far z"), farZ)
SERIALIZED_TYPE(xorstr_("Max density"), maxDensity)
SERIALIZED_TYPE(xorstr_("HDR color scale"), hdrColorScale)
SERIALIZED_TYPE(xorstr_("Zoom fog scale"), zoomFogScale)
END_SERIALIZED_STRUCT
