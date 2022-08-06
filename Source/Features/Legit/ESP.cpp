#include "ESP.hpp"

#include "xorstr.hpp"

#include "../../Interfaces.hpp"
#include "../../SDK/GameClass/CBasePlayer.hpp"

#include "../../Hooks/FrameStageNotify/FrameStageNotifyHook.hpp"

// Settings
bool Features::Legit::Esp::enabled = false;

bool WorldToScreen(Matrix4x4& matrix, Vector worldPosition, ImVec2& screenPosition)
{
    float w = matrix[3][0] * worldPosition.x + matrix[3][1] * worldPosition.y + matrix[3][2] * worldPosition.z + matrix[3][3];
    if (w < 0.001f)
        return false;

    screenPosition = ImGui::GetIO().DisplaySize;
    screenPosition.x /= 2.0f;
	screenPosition.y /= 2.0f;
    
    screenPosition.x *= 1.0f + (matrix[0][0] * worldPosition.x + matrix[0][1] * worldPosition.y + matrix[0][2] * worldPosition.z + matrix[0][3]) / w;
    screenPosition.y *= 1.0f - (matrix[1][0] * worldPosition.x + matrix[1][1] * worldPosition.y + matrix[1][2] * worldPosition.z + matrix[1][3]) / w;
    return true;
}

void Features::Legit::Esp::ImGuiRender(ImDrawList* drawList) {
	if(!enabled)
		return;
		
	if(!Interfaces::engine->IsInGame())
		return;
		
	Matrix4x4 matrix = Hooks::FrameStageNotify::worldToScreenMatrix;

	if(!matrix.Base())
		return;

	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n----\n",
		matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
		matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
		matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
	
	int localPlayerIndex = Interfaces::engine->GetLocalPlayer();
	C_BasePlayer* localPlayer = reinterpret_cast<C_BasePlayer*>(Interfaces::entityList->GetClientEntity(localPlayerIndex));
	
	// The first object is always the WorldObj
	for(int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		C_BasePlayer* player = reinterpret_cast<C_BasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if(!player ||
			player == localPlayer ||
			player->GetDormant() ||
			*player->LifeState() != LIFE_ALIVE
		) continue;

		C_Collideable* collideable = player->GetCollideable();

		Vector min = *collideable->ObbMins();
		Vector max = *collideable->ObbMaxs();
		
		ImVec2 head2D;
		ImVec2 feet2D;
		
		if(!WorldToScreen(matrix, min, head2D))
			continue;
		if(!WorldToScreen(matrix, max, feet2D))
			continue;

		drawList->AddRect(head2D, feet2D, ImColor(255.f, 255.f, 0.f, 255.f));
	}
}

void Features::Legit::Esp::SetupGUI() {
	ImGui::Checkbox(xorstr_("Enabled##ESP"), &enabled);
}
