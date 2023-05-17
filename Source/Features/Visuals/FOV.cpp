#include "Visuals.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GameCache.hpp"
#include "../../GUI/Elements/HelpMarker.hpp"
#include "../../Interfaces.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <vector>

static bool forceFOV = false;
static bool ignoreScoped = true;
static float fov = 90.0f;
static bool forceViewModel = false;
static float viewModelFovOffset = 0.0f;
static bool viewOffset = true;
static float offsetX = 0.0f;
static float offsetY = 0.0f;
static float offsetZ = 0.0f;
static float rotationOffsetX = 0.0f;
static float rotationOffsetY = 0.0f;
static float rotationOffsetZ = 0.0f;

void Features::Visuals::FOV::OverrideView(CViewSetup* pSetup)
{
	if ((!forceFOV && !forceViewModel) || !Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	CBasePlayer* viewer = localPlayer;

	if (!viewer)
		return;

	if (!localPlayer->IsAlive() && *viewer->ObserverMode() == ObserverMode::OBS_MODE_IN_EYE && viewer->ObserverTarget()) {
		auto* observerTarget = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntityFromHandle(viewer->ObserverTarget()));
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

void Features::Visuals::FOV::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Force FOV"), &forceFOV);
	if (forceFOV) {
		ImGui::Checkbox(xorstr_("Ignore scoped"), &ignoreScoped);
		ImGui::SliderFloat(xorstr_("FOV"), &fov, 0.0f, 180.0f);
	}

	ImGui::Checkbox(xorstr_("Force View model"), &forceViewModel);
	if (forceViewModel) {
		ImGui::SliderFloat(xorstr_("View model FOV offset"), &viewModelFovOffset, -90.0f, 90.0f);

		ImGui::Checkbox(xorstr_("View offset"), &viewOffset);
		ImGui::HelpMarker(xorstr_("Should the offset be a literal 3D movement?"));

		ImGui::DragFloat(xorstr_("Offset X"), &offsetX, 0.1f);
		ImGui::DragFloat(xorstr_("Offset Y"), &offsetY, 0.1f);
		ImGui::DragFloat(xorstr_("Offset Z"), &offsetZ, 0.1f);

		ImGui::SliderFloat(xorstr_("Rotation offset X"), &rotationOffsetX, -90.0f, 90.0f);
		ImGui::SliderFloat(xorstr_("Rotation offset Y"), &rotationOffsetY, -90.0f, 90.0f);
		ImGui::SliderFloat(xorstr_("Rotation offset Z"), &rotationOffsetZ, -90.0f, 90.0f);
	}
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::FOV::Serializer)
SERIALIZED_TYPE(xorstr_("Force FOV"), forceFOV)
SERIALIZED_TYPE(xorstr_("Ignore scoped"), ignoreScoped)
SERIALIZED_TYPE(xorstr_("FOV"), fov)
SERIALIZED_TYPE(xorstr_("Force View model"), forceViewModel)
SERIALIZED_TYPE(xorstr_("View model FOV"), viewModelFovOffset)
SERIALIZED_TYPE(xorstr_("View offset"), viewOffset)
SERIALIZED_TYPE(xorstr_("Offset X"), offsetX)
SERIALIZED_TYPE(xorstr_("Offset Y"), offsetY)
SERIALIZED_TYPE(xorstr_("Offset Z"), offsetZ)
SERIALIZED_TYPE(xorstr_("Rotation offset X"), rotationOffsetX)
SERIALIZED_TYPE(xorstr_("Rotation offset Y"), rotationOffsetY)
SERIALIZED_TYPE(xorstr_("Rotation offset Z"), rotationOffsetZ)
END_SERIALIZED_STRUCT
