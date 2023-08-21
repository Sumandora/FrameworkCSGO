#include "Fog.hpp"

#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../SDK/GameClass/Entities/CFogController.hpp"

// TODO Reset the fog when turned off
// TODO also some maps might not have a fog controller (?)

void Fog::frameStageNotify() const
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

void Fog::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);

	ImGui::Separator();

	ImGui::Checkbox("Hide fog", &hideFog);

	if (hideFog)
		return;

	ImGui::ClickableColorButton("Color primary", colorPrimary);

	ImGui::Checkbox("Blend", &blend);

	if (blend) {
		ImGui::DragFloat3("Dir Primary", dirPrimary.Base(), 0.01f, -1.0f, 1.0f);
		ImGui::ClickableColorButton("Color secondary", colorSecondary);
	}

	ImGui::SliderFloat("Start", &start, 0.0f, 1024.0f, "%.2f");
	ImGui::SliderFloat("End", &end, 0.0f, 1024.0f, "%.2f");
	ImGui::SliderFloat("Far z", &farZ, 0.0f, 1024.0f * 16, "%.2f");
	ImGui::SliderFloat("Max density", &maxDensity, 0.0f, 1.0f, "%.2f");

	ImGui::SliderFloat("HDR color scale", &hdrColorScale, 0.0f, 1.0f, "%.2f");
	ImGui::SliderFloat("Zoom fog scale", &zoomFogScale, 0.0f, 1.0f, "%.2f");
}

SCOPED_SERIALIZER(Fog)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Hide fog", hideFog);
	SERIALIZE_VECTOR4D("Color primary", colorPrimary.Value);
	SERIALIZE("Blend", blend);
	SERIALIZE_VECTOR3D("Dir Primary", dirPrimary);
	SERIALIZE_VECTOR4D("Color secondary", colorSecondary.Value);
	SERIALIZE("Start", start);
	SERIALIZE("End", end);
	SERIALIZE("Far z", farZ);
	SERIALIZE("Max density", maxDensity);
	SERIALIZE("HDR color scale", hdrColorScale);
	SERIALIZE("Zoom fog scale", zoomFogScale);
}
