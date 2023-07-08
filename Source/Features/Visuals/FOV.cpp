#include "FOV.hpp"

#include "imgui.h"

#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../Interfaces.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <vector>

// TODO Aspect ratio

void FOV::OverrideView(CViewSetup* pSetup)
{
	if ((!forceFOV && !forceViewModel) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	CBasePlayer* viewer = localPlayer;

	if (!viewer)
		return;

	if (!localPlayer->IsAlive() && *viewer->ObserverMode() == ObserverMode::OBS_MODE_IN_EYE && viewer->ObserverTarget()) {
		auto* observerTarget = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntityFromHandle(viewer->ObserverTarget()));
		if (observerTarget && observerTarget->IsAlive())
			viewer = observerTarget;
		else
			return;
	}

	if (forceFOV && (!ignoreScoped || !*viewer->Scoped()))
		pSetup->fov = fov;

	if (viewer != localPlayer)
		return; // View model movement doesn't work when spectating (flickering)

	if (forceViewModel && !*viewer->Scoped() /* Don't move scoped weapons around, that will just offset the scope. No reason why you would want that. */) {
		CBaseEntity* viewModel = Interfaces::entityList->GetClientEntityFromHandle(viewer->ViewModel());
		if (viewModel) {
			const bool isForcingFOV = viewModelFovOffset != 0;
			const bool isForcingOffset = offsetX != 0 || offsetY != 0 || offsetZ != 0;

			if (isForcingFOV || isForcingOffset) {
				Vector viewAngles{};
				Interfaces::engine->GetViewAngles(&viewAngles);

				Vector forward{};
				Vector right{};
				Vector up{};
				Utils::AngleVectors(viewAngles, &forward, &right, &up);

				Vector& origin = viewModel->GetRenderOrigin();

				if (isForcingFOV) // This is technically the same as enabling viewOffset and setting offsetY to your fov
					origin += (forward * viewModelFovOffset);

				if (isForcingOffset) {
					if (viewOffset) {
						origin += (forward * offsetY) + (up * offsetZ) + (right * offsetX); // https://github.com/SwagSoftware/Kisak-Strike/blob/4c2fdc31432b4f5b911546c8c0d499a9cff68a85/game/shared/baseviewmodel_shared.cpp#L478
					} else {
						origin.x += offsetX;
						origin.y += offsetY;
						origin.z += offsetZ;
					}
				}
			}

			if (rotationOffsetX != 0 || rotationOffsetY != 0 || rotationOffsetZ != 0) {
				Vector& angles = viewModel->GetRenderAngles();
				angles.x += rotationOffsetX;
				angles.y += rotationOffsetY;
				angles.z += rotationOffsetZ;
			}
		}
	}
}

void FOV::SetupGUI()
{
	ImGui::Checkbox("Force FOV", &forceFOV);
	if (forceFOV) {
		ImGui::Checkbox("Ignore scoped", &ignoreScoped);
		ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f);
	}

	ImGui::Checkbox("Force View model", &forceViewModel);
	if (forceViewModel) {
		ImGui::SliderFloat("View model FOV offset", &viewModelFovOffset, -90.0f, 90.0f);

		ImGui::Checkbox("View offset", &viewOffset);
		ImGui::HelpMarker("Should the offset be a literal 3D movement?");

		ImGui::DragFloat("Offset X", &offsetX, 0.1f);
		ImGui::DragFloat("Offset Y", &offsetY, 0.1f);
		ImGui::DragFloat("Offset Z", &offsetZ, 0.1f);

		ImGui::SliderFloat("Rotation offset X", &rotationOffsetX, -90.0f, 90.0f);
		ImGui::SliderFloat("Rotation offset Y", &rotationOffsetY, -90.0f, 90.0f);
		ImGui::SliderFloat("Rotation offset Z", &rotationOffsetZ, -90.0f, 90.0f);
	}
}

SCOPED_SERIALIZER(FOV)
{
	SERIALIZE("Force FOV", forceFOV);
	SERIALIZE("Ignore scoped", ignoreScoped);
	SERIALIZE("FOV", fov);
	SERIALIZE("Force View model", forceViewModel);
	SERIALIZE("View model FOV", viewModelFovOffset);
	SERIALIZE("View offset", viewOffset);
	SERIALIZE("Offset X", offsetX); // TODO Make vectors?
	SERIALIZE("Offset Y", offsetY);
	SERIALIZE("Offset Z", offsetZ);
	SERIALIZE("Rotation offset X", rotationOffsetX);
	SERIALIZE("Rotation offset Y", rotationOffsetY);
	SERIALIZE("Rotation offset Z", rotationOffsetZ);
}
