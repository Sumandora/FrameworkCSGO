#include "../Debug.hpp"

#include "../../../../Hooks/Game/GameFunctions.hpp"

#include "imgui.h"

void Gui::Windows::GameEvents()
{
	ImGui::Text(xorstr_("This log will be extended, whenever a new event is fired"));
	ImGui::Separator();
	for (const auto& [name, count] : Hooks::Game::FireEvent::events)
		ImGui::Text(xorstr_("%s (%d)"), name.c_str(), count);
}
