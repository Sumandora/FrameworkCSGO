#include "../Debug.hpp"

#include "../../../../Hooks/Game/GameFunctions.hpp"

#include "imgui.h"

void Gui::Windows::gameEvents()
{
	ImGui::Text("This log will be extended, whenever a new event is fired");
	ImGui::Separator();
	for (const auto& [name, count] : Hooks::Game::FireEvent::events)
		ImGui::Text("%s (%d)", name.c_str(), count);
}
