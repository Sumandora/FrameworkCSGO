#include "ESP.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "../../GameCache.hpp"
#include "../../Interfaces.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include "../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

#include <vector>

bool  Features::Legit::Esp::enabled			 = false;
float Features::Legit::Esp::rounding		 = 0.0f;
float Features::Legit::Esp::thickness		 = 1.0f;
bool  Features::Legit::Esp::outlined		 = false;
float Features::Legit::Esp::outlineThickness = 1.0f;

bool WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition) {
	float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
	if (w < 0.01f)
		return false;

	screenPosition	 = ImVec2(ImGui::GetIO().DisplaySize);
	screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;

	screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
	screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
	return true;
}

void Features::Legit::Esp::ImGuiRender(ImDrawList* drawList) {
	if (!enabled)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	Matrix4x4 matrix = Hooks::FrameStageNotify::worldToScreenMatrix;

	if (!matrix.Base())
		return;

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == GameCache::GetLocalPlayer() || player->GetDormant() || *player->LifeState() != LIFE_ALIVE)
			continue;

		CCollideable* collideable = player->Collision();

		Vector min = *player->VecOrigin() + *collideable->ObbMins();
		Vector max = *player->VecOrigin() + *collideable->ObbMaxs();

		Vector points[] = {
			// Lower
			Vector(min.x, min.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, min.y, max.z),
			Vector(min.x, min.y, max.z),
			// Higher
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z)
		};

		auto   topLeft = ImVec2(ImGui::GetIO().DisplaySize); // hacky but hey, it works
		ImVec2 bottomRight;
		bool   visible = true;

		for (const auto& point : points) {
			ImVec2 point2D;

			if (!WorldToScreen(matrix, point, point2D)) {
				visible = false;
				break;
			}

			if (point2D.x < topLeft.x)
				topLeft.x = point2D.x;

			if (point2D.y < topLeft.y)
				topLeft.y = point2D.y;

			if (point2D.x > bottomRight.x)
				bottomRight.x = point2D.x;

			if (point2D.y > bottomRight.y)
				bottomRight.y = point2D.y;
		}

		if (visible) {
			if (outlined)
				drawList->AddRect(topLeft, bottomRight, ImGuiColors::black, rounding, ImDrawFlags_None, thickness + outlineThickness);
			drawList->AddRect(topLeft, bottomRight, ImGuiColors::white, rounding, ImDrawFlags_None, thickness);
		}
	}
}

void Features::Legit::Esp::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##LegitESP"), &enabled);
	ImGui::SliderFloat(xorstr_("Rounding##LegitESP"), &rounding, 0.0f, 10.0f, "%.2f");
	ImGui::SliderFloat(xorstr_("Thickness##LegitESP"), &thickness, 0.0f, 10.0f, "%.2f");
	ImGui::Checkbox(xorstr_("Outlined##LegitESP"), &outlined);
	if (outlined)
		ImGui::SliderFloat(xorstr_("Outline thickness##LegitESP"), &outlineThickness, 0.0f, 10.0f, "%.2f");
}
