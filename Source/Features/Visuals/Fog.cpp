#include "Fog.hpp"

#include "../../Interfaces.hpp"

#include "../../GUI/Elements/ClickableColorButton.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../SDK/GameClass/CFogController.hpp"

// TODO Reset the fog when turned off
// TODO also some maps might not have a fog controller (?)

void Fog::FrameStageNotify()
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

void Fog::SetupGUI()
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

SCOPED_SERIALIZER(Fog)
{
	SERIALIZE(xorstr_("Enabled"), enabled);
	SERIALIZE(xorstr_("Hide fog"), hideFog);
	SERIALIZE_VECTOR4D(xorstr_("Color primary"), colorPrimary.Value);
	SERIALIZE(xorstr_("Blend"), blend);
	SERIALIZE_VECTOR3D(xorstr_("Dir Primary"), dirPrimary);
	SERIALIZE_VECTOR4D(xorstr_("Color secondary"), colorSecondary.Value);
	SERIALIZE(xorstr_("Start"), start);
	SERIALIZE(xorstr_("End"), end);
	SERIALIZE(xorstr_("Far z"), farZ);
	SERIALIZE(xorstr_("Max density"), maxDensity);
	SERIALIZE(xorstr_("HDR color scale"), hdrColorScale);
	SERIALIZE(xorstr_("Zoom fog scale"), zoomFogScale);
}
