#include "../Glow.hpp"

void Glow::Player::Apply(int entindex, CBasePlayer* player)
{
	CBasePlayer* localPlayer = Memory::GetLocalPlayer();
	if (player == localPlayer)
		local.Apply(entindex, player);
	else if (*player->Team() == *localPlayer->Team())
		teammate.Apply(entindex, player);
	else
		enemy.Apply(entindex, player);
}

void Glow::Player::SetupGUI()
{
	if (ImGui::BeginTabBar("#Player config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Teammate")) {
			teammate.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Enemy")) {
			enemy.SetupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Local")) {
			local.SetupGUI();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

SCOPED_SERIALIZER(Glow::Player)
{
	SERIALIZE_STRUCT("Teammate", teammate);
	SERIALIZE_STRUCT("Enemy", enemy);
	SERIALIZE_STRUCT("Local", local);
}