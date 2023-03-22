#include "../Visuals.hpp"

#include "../../Interfaces.hpp"
#include "imgui.h"
#include "xorstr.hpp"

#include "../../Utils/Trigonometry.hpp"

#include <vector>

bool Features::Visuals::FOV::forceFOV = false;
bool Features::Visuals::FOV::ignoreScoped = true;
float Features::Visuals::FOV::fov = 90.0f;
bool Features::Visuals::FOV::forceViewModel = false;
float Features::Visuals::FOV::viewModelFovOffset = 0.0f;
bool Features::Visuals::FOV::viewOffset = true;
float Features::Visuals::FOV::offsetX = 0.0f;
float Features::Visuals::FOV::offsetY = 0.0f;
float Features::Visuals::FOV::offsetZ = 0.0f;
float Features::Visuals::FOV::rotationOffsetX = 0.0f;
float Features::Visuals::FOV::rotationOffsetY = 0.0f;
float Features::Visuals::FOV::rotationOffsetZ = 0.0f;

void Features::Visuals::FOV::OverrideView(CViewSetup* pSetup)
{
	CBasePlayer* viewTarget = GameCache::GetLocalPlayer();
	if (!viewTarget || *viewTarget->LifeState() != LIFE_ALIVE)
		return;

	if (forceFOV && (!ignoreScoped || !*viewTarget->Scoped()))
		pSetup->fov = fov;

	if (forceViewModel) {
		CBaseEntity* viewModel = Interfaces::entityList->GetClientEntityFromHandle(viewTarget->ViewModel());
		if (viewModel) {
			bool isForcingFOV = viewModelFovOffset != 0;
			bool isForcingOffset = offsetX != 0 || offsetY != 0 || offsetZ != 0;
			if (isForcingFOV || isForcingOffset) {
				Vector viewAngles {};
				Interfaces::engine->GetViewAngles(&viewAngles);

				Vector forward {};
				Vector right {};
				Vector up {};
				Utils::AngleVectors(viewAngles, &forward, &right, &up);

				Vector& origin = viewModel->GetRenderOrigin();

				if(isForcingFOV) // This is technically the same as enabling viewOffset and setting offsetY to your fov
					origin += (forward * viewModelFovOffset);

				if(isForcingOffset) {
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
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(xorstr_("Should the offset be a literal 3D movement?"));

		ImGui::SliderFloat(xorstr_("Offset X"), &offsetX, -10.0f, 10.0f);
		ImGui::SliderFloat(xorstr_("Offset Y"), &offsetY, -10.0f, 10.0f);
		ImGui::SliderFloat(xorstr_("Offset Z"), &offsetZ, -10.0f, 10.0f);

		ImGui::SliderFloat(xorstr_("Rotation offset X"), &rotationOffsetX, -90.0f, 90.0f);
		ImGui::SliderFloat(xorstr_("Rotation offset Y"), &rotationOffsetY, -90.0f, 90.0f);
		ImGui::SliderFloat(xorstr_("Rotation offset Z"), &rotationOffsetZ, -90.0f, 90.0f);
	}
}

BEGIN_SERIALIZED_STRUCT(Features::Visuals::FOV::Serializer, xorstr_("FOV"))
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
