#include "../Glow.hpp"

bool Glow::Player::apply(int entindex, CBasePlayer* player) const
{
	CBasePlayer* localPlayer = Memory::getLocalPlayer();
	if (player == localPlayer)
		return local.apply(entindex, player);
	else if (!player->IsEnemy(localPlayer))
		return teammate.apply(entindex, player);
	else
		return enemy.apply(entindex, player);
}

void Glow::Player::setupGUI()
{
	if (ImGui::BeginTabBar("#Player config selection", ImGuiTabBarFlags_Reorderable)) {
		if (ImGui::BeginTabItem("Teammate")) {
			teammate.setupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Enemy")) {
			enemy.setupGUI();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Local")) {
			local.setupGUI();
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